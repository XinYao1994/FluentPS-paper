#include "ps/ps.h"
#include <unistd.h>
#include <math.h>
using namespace ps;
using namespace std;

#define HAVE_CAFFE

#define PrintSyc

// SSP connection test
// template <typename Val>
// struct KVServerSSPHandle {
//using WorkerId = uint64_t;
using Staleness = uint64_t;
using Callback = std::function<void()>;

template<class Val>
class KVServerSSPHandle {
public:
	/**
	 *req_meta: the meta info about this request, including cmd push sender timestamp customer_id
	 req_data: the data, key, value, value lens
	 server: the pointer of the current PS
	 */
	void operator()(const KVMeta& req_meta, const KVPairs<Val>& req_data,
			KVServer<Val>* server) {
		size_t n = req_data.keys.size();
		KVPairs<Val> res;
		if (req_meta.push) {
			CHECK_EQ(n, req_data.vals.size());
		} else {
			res.keys = req_data.keys;
			res.vals.resize(n);
		}
		int current_iter = req_meta.staleness;
		Key skey = req_data.keys[0];
		if (req_meta.push) {
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				store[key] += req_data.vals[i];
			}
			workercount[current_iter] += 1; // For this iteration, add one
			while (workercount[ticks[skey]] == NumWorkers()) { // For a given key, if the staleness has been passed number of workers, add one
				//trigger a cb of pull
				auto& cbs = callbacks_[ticks[skey]];
				for (const auto& cb : cbs) {
					cb();
				}
				ticks[skey] += 1;
			}
		} else {
			/*
			 * SSP condition
			 * the slowest one + stale <= current_iter
			 * we can not pull data until
			 */
			if (ticks[skey] + server->stale <= current_iter) { // Wait
				//wait for the slow workers catch up
				callbacks_[ticks[skey]].push_back(
						[this, req_meta, req_data, res, server]() mutable {
							size_t n = req_data.keys.size();
							for (size_t i = 0; i < n; ++i) {
								Key key = req_data.keys[i];
								res.vals[i] = store[key];
							}
							server->Response(req_meta, res);
						});
				return;
			}
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				res.vals[i] = store[key];
			}
		}
		server->Response(req_meta, res);
	}
public:
	unordered_map<Key, Val> store;
	unordered_map<Key, Staleness> ticks;
	unordered_map<Staleness, int> workercount;
	unordered_map<Staleness, std::vector<Callback>> callbacks_;

};

//ticks[skey] + server->stale <= current_iter || callbacks_[current_iter].size() != 0
bool SSP_con_stream(int num_pass_workers, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = s_gap <= k;
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return  ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

bool SSP_con(int num_pass_workers, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = num_pass_workers != 0 ||  s_gap <= k;
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return  ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

template<class Val>
class KVServerSSPHandle_Caffe {
public:
	/**
	 *req_meta: the meta info about this request, including cmd push sender timestamp customer_id
	 req_data: the data, key, value, value lens
	 server: the pointer of the current PS
	 */
	void operator()(const KVMeta& req_meta, const KVPairs<Val>& req_data,
			KVServer<Val>* server) {
		size_t n = req_data.keys.size();
		int num_worker = NumWorkers();
		KVPairs<Val> res;
		if (req_meta.push) {
			CHECK_EQ(n, req_data.lens.size());// matching key array with lens array. key->value->lens
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				lens += req_data.lens[i];
			}
			size_t v_size = req_data.vals.size();
			cout << "record: "<< lens << ", val size: " << v_size << endl; // lens VS vals.size()
			CHECK_EQ(lens, req_data.vals.size());
		} else {
			res.keys = req_data.keys;
			res.lens.resize(n);
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				lens += store[key].size();
			}
			res.vals.resize(lens);
		}
		int current_iter = req_meta.staleness;
		Key skey = req_data.keys[0];
		if (req_meta.push) {
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				size_t lens = req_data.lens[i];
				// checking values with lens CHECK_EQ(n, req_data.lens.size());
				Key key = req_data.keys[i];
				if (!store[key].size()) {
					for (size_t j = 0; j < lens; j++) {
						store[key].push_back(
								req_data.vals[tolal_lens + j] / num_worker); // init
					}
				} else {
					for (size_t j = 0; j < lens; j++) {
						store[key][j] += (req_data.vals[tolal_lens + j]
								/ num_worker); //further add
					}
				}
				tolal_lens += lens;
			}
			workercount[current_iter] += 1; // For this iteration, add one
			//std::cout << "push" << ticks[skey] << " " << current_iter << " " << workercount[ticks[skey]]<< std::endl;
			if (workercount[ticks[skey]] == num_worker) { // For a given key, if the staleness has been passed number of workers, add one
				//trigger a cb of pull
				//std:pair<Key, Staleness> _callindex = std::make_pair(skey, ticks[skey]);
				//std::cout << "push release pulls"<< " "<< ticks[skey]<<" " << callbacks_[_callindex].size()<< std::endl;
				//auto& cbs = callbacks_[_callindex];
				//std::cout << "push release pulls"<< " "<< ticks[skey]<<" " << callbacks_[ticks[skey]].size()<< std::endl;
				auto& cbs = callbacks_[ticks[skey]];
				for (const auto& cb : cbs) {
					cb();
				}
				//erase anything, just release the memory
				//callbacks_.erase(_callindex);
				callbacks_.erase(ticks[skey]);
				ticks[skey] += 1;
			}
		} else {
			/*
			 * SSP condition
			 * the slowest one + stale <= current_iter
			 * we can not pull data until
			 */
			if (ticks[skey] + server->stale <= current_iter || callbacks_[current_iter].size() != 0) { // Wait
				//wait for the slow workers catch up
				callbacks_[current_iter].push_back(
						[this, req_meta, req_data, res, server]() mutable {
							size_t n = req_data.keys.size();
							size_t tolal_lens = 0;
							for (size_t i = 0; i < n; ++i) {
								Key key = req_data.keys[i];
								size_t lens = store[key].size();
								res.lens[i] = lens;
								for (size_t j = 0; j < lens; j++) {
									res.vals[tolal_lens + j] = store[key][j];
								}
								tolal_lens += lens;
							}
							server->Response(req_meta, res);
						});
				return;
			}
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				size_t lens = store[key].size();
				res.lens[i] = lens;
				for (size_t j = 0; j < lens; j++) {
					res.vals[tolal_lens + j] = store[key][j];
				}
				tolal_lens += lens;
			}
		}
		server->Response(req_meta, res);
	}
public:
	unordered_map<Key, std::vector<Val>> store;
	unordered_map<Key, Staleness> ticks;
	unordered_map<Staleness, int> workercount;
	//unordered_map<std::pair<Key, Staleness>, std::vector<Callback>> callbacks_;
	unordered_map<Staleness, std::vector<Callback>> callbacks_;

};

template<class Val>
class KVServerSSPHandle_Caffe_Simple {
public:
	/**
	 *req_meta: the meta info about this request, including cmd push sender timestamp customer_id
	 req_data: the data, key, value, value lens
	 server: the pointer of the current PS
	 */
	void operator()(const KVMeta& req_meta, const KVPairs<Val>& req_data,
			KVServer<Val>* server) {
		size_t n = req_data.keys.size();
		int num_worker = NumWorkers();
		int current_iter = req_meta.staleness;
		Key skey = req_data.keys[0];
		if (req_meta.push) {
			// Check Push
			CHECK_EQ(n, req_data.lens.size());// matching key array with lens array. key->value->lens
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				lens += req_data.lens[i];
			}
			size_t v_size = req_data.vals.size();
			//cout << "record: "<< lens << ", val size: " << v_size << endl; // lens VS vals.size()
			CHECK_EQ(lens, req_data.vals.size());

			// Execute Push
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				size_t lens = req_data.lens[i];
				// checking values with lens CHECK_EQ(n, req_data.lens.size());
				Key key = req_data.keys[i];
				if (!store[key].size()) {
					req_key.push_back(key);
					for (size_t j = 0; j < lens; j++) {
						store[key].push_back(
								req_data.vals[tolal_lens + j] / num_worker); // init
					}
					ticks[skey] = current_iter;
					//cout << "begining iter" << current_iter << endl;
				} else {
					for (size_t j = 0; j < lens; j++) {
						store[key][j] += (req_data.vals[tolal_lens + j]
								/ num_worker); //further add
					}
				}
				tolal_lens += lens;
			}
			workercount[current_iter] += 1; // For this iteration, add one
			//std::cout << "push" << ticks[skey] << " " << current_iter << " " << workercount[ticks[skey]]<< std::endl;
			if (workercount[ticks[skey]] == num_worker) { // For a given key, if the staleness has been passed number of workers, add one
				//trigger a cb of pull
				auto& cbs = callbacks_[ticks[skey]];
				for (const auto& cb : cbs) {
					cb();
				}
				//erase anything, just release the memory
				callbacks_.erase(ticks[skey]);
				ticks[skey] += 1;
			}
			server->Response(req_meta, respush);
		} else {
			respull.keys = req_data.keys;
			/*
			 * SSP condition
			 * the slowest one + stale <= current_iter
			 * we can not pull data until
			 * ticks[skey] + server->stale <= current_iter || callbacks_[current_iter].size() != 0
			 * SSP_con(callbacks_[current_iter].size(), server->stale, current_iter, ticks[skey])
			 */
			if (SSP_con_stream(callbacks_[current_iter].size(), server->stale, current_iter, ticks[skey])) { // Wait
				//wait for the slow workers catch up
				callbacks_[ticks[skey]].push_back(
				//callbacks_[current_iter].push_back(
						[this, req_meta, server]() mutable {
							// Check pull
							size_t n = req_key.size();
							respull.lens.resize(n);
							size_t lens = 0;
							for (size_t i = 0; i < n; ++i) {
								lens += store[req_key[i]].size();
							}
							respull.vals.resize(lens);
							// execute pull
							size_t tolal_lens = 0;
							for (size_t i = 0; i < n; ++i) {
								size_t lens = store[req_key[i]].size();
								respull.lens[i] = lens;
								for (size_t j = 0; j < lens; j++) {
									respull.vals[tolal_lens + j] = store[req_key[i]][j];
								}
								tolal_lens += lens;
							}
							server->Response(req_meta, respull);
						});
				return;
			}
			// Check pull
			respull.lens.resize(n);
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				lens += store[key].size();
			}
			respull.vals.resize(lens);
			// execute pull
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				size_t lens = store[key].size();
				respull.lens[i] = lens;
				for (size_t j = 0; j < lens; j++) {
					respull.vals[tolal_lens + j] = store[key][j];
				}
				tolal_lens += lens;
			}
			server->Response(req_meta, respull);
		}
	}
public:
	unordered_map<Key, std::vector<Val>> store;
	unordered_map<Key, Staleness> ticks;
	unordered_map<Staleness, int> workercount;
	std::vector<Key> req_key;
	KVPairs<Val> respull;
	KVPairs<Val> respush;
	unordered_map<Staleness, std::vector<Callback>> callbacks_;
};

bool PSSP_con(int s, int k) {
	return k > 0 && s <= k && (((double) rand()) / RAND_MAX)
			<= (double(1) / (double(1) + double(exp(s - k))));
}

template<class Val>
class KVServerPSSPHandle_Caffe {
public:
	/**
	 *req_meta: the meta info about this request, including cmd push sender timestamp customer_id
	 req_data: the data, key, value, value lens
	 server: the pointer of the current PS
	 */
	void operator()(const KVMeta& req_meta, const KVPairs<Val>& req_data,
			KVServer<Val>* server) {
		size_t n = req_data.keys.size();
		int num_worker = NumWorkers();
		KVPairs<Val> res;
		if (req_meta.push) {
			CHECK_EQ(n, req_data.lens.size());// matching key array with lens array. key->value->lens
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				lens += req_data.lens[i];
			}
			size_t v_size = req_data.vals.size();
			cout << "record: "<< lens << ", val size: " << v_size << endl;
			CHECK_EQ(lens, req_data.vals.size());
		} else {
			res.keys = req_data.keys;
			res.lens.resize(n);
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				lens += store[key].size();
			}
			res.vals.resize(lens);
		}
		int current_iter = req_meta.staleness;
		Key skey = req_data.keys[0];
		if (req_meta.push) {
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				size_t lens = req_data.lens[i];
				// checking values with lens CHECK_EQ(n, req_data.lens.size());
				Key key = req_data.keys[i];
				if (!store[key].size()) {
					for (size_t j = 0; j < lens; j++) {
						store[key].push_back(
								req_data.vals[tolal_lens + j] / num_worker); // init
					}
				} else {
					for (size_t j = 0; j < lens; j++) {
						store[key][j] += (req_data.vals[tolal_lens + j]
								/ num_worker); //further add
					}
				}
				tolal_lens += lens;
			}
			workercount[current_iter] += 1; // For this iteration, add one
			while (workercount[ticks[skey]] == num_worker) { // For a given key, if the staleness has been passed number of workers, add one
				//trigger a cb of pull
				auto& cbs = callbacks_[ticks[skey]];
				for (const auto& cb : cbs) {
					cb();
				}
				//erase anything, just release the memory
				callbacks_.erase(ticks[skey]);
				ticks[skey] += 1;
			}
		} else {
			/*
			 * Probabilistic SSP condition
			 * pull data with $P(k)=1/(1+e^{(s-k)})$
			 */
			if (PSSP_con(server->stale, current_iter - ticks[skey])) { // Wait
				//wait for the slow workers catch up
				callbacks_[ticks[skey]].push_back(
						[this, req_meta, req_data, res, server]() mutable {
							size_t n = req_data.keys.size();
							size_t tolal_lens = 0;
							for (size_t i = 0; i < n; ++i) {
								Key key = req_data.keys[i];
								size_t lens = store[key].size();
								res.lens[i] = lens;
								for (size_t j = 0; j < lens; j++) {
									res.vals[tolal_lens + j] = store[key][j];
								}
								tolal_lens += lens;
							}
							server->Response(req_meta, res);
						});
				return;
			}
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				size_t lens = store[key].size();
				res.lens[i] = lens;
				for (size_t j = 0; j < lens; j++) {
					res.vals[tolal_lens + j] = store[key][j];
				}
				tolal_lens += lens;
			}
		}
		server->Response(req_meta, res);
	}
public:
	unordered_map<Key, std::vector<Val>> store;
	unordered_map<Key, Staleness> ticks;
	unordered_map<Staleness, int> workercount;
	unordered_map<Staleness, std::vector<Callback>> callbacks_;

};

double Pro = 1.0;

bool SPSSP_con_stream(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = (fastest == current && s_gap <= k && (((double) rand()) / RAND_MAX) <= Pro);
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return  ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

bool DPSSP_con_stream(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = (fastest == current && s_gap <= k && (((double) rand()) / RAND_MAX) <= (double(1) / (double(1) + double(exp(s_gap - k)))));
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return  ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

bool DPSSP_con_stream_2s(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = (fastest == current && s_gap <= k && (((double) rand()) / RAND_MAX) <= (double(1) / (double(1) + double(exp(2 * s_gap - k)))));
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return  ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

bool DPSSP_con_stream_tmp(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
        int k = current - slowest;
        bool ret = (fastest == current && 0 <= k && (((double) rand()) / RAND_MAX) <= (double(1) / (double(1) + double(exp(s_gap - k)))));
#ifdef PrintSyc
        if(ret == true){
                std::cout << "sync at " << current << std::endl;
        }
#endif
        return  ret;
}

bool DPSSP_con_stream_tmp_cg(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
        int k = current - slowest;
        bool ret = (fastest == current && 0 <= k && (((double) rand()) / RAND_MAX) <= ((log10(current)-1) / (double(1) + double(exp(s_gap - k)))));
#ifdef PrintSyc
        if(ret == true){
                std::cout << "sync at " << current << std::endl;
        }
#endif
        return  ret;
}

bool SPSSP_con(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = num_pass_workers != 0 || (fastest == current && s_gap <= k && (((double) rand()) / RAND_MAX) <= Pro);
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

bool DPSSP_con(int num_pass_workers, int fastest, int s_gap, int current, int slowest) {
	int k = current - slowest;
	bool ret = num_pass_workers != 0 || (fastest == current && s_gap <= k && (((double) rand()) / RAND_MAX) <= (double(1) / (double(1) + double(exp(s_gap - k)))));
#ifdef PrintSyc
	if(ret == true){
		std::cout << "sync at " << current << std::endl;
	}
#endif
	return  ret;
			//k > 0 && s <= k && (((double) rand()) / RAND_MAX) <= Pro; num_pass_workers != 0 ||
}

template<class Val>
class KVServerPSSPHandle_Caffe_Simple {
public:
	/**
	 *req_meta: the meta info about this request, including cmd push sender timestamp customer_id
	 req_data: the data, key, value, value lens
	 server: the pointer of the current PS
	 */
	void operator()(const KVMeta& req_meta, const KVPairs<Val>& req_data,
			KVServer<Val>* server) {
		size_t n = req_data.keys.size();
		int num_worker = NumWorkers();
		double factor = Pro < 0.3 ? (double) num_worker : (double) num_worker * (double) sqrt(Pro);
		int current_iter = req_meta.staleness;
		Key skey = req_data.keys[0];
		if (req_meta.push) {
			// Check Push
			CHECK_EQ(n, req_data.lens.size());// matching key array with lens array. key->value->lens
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				lens += req_data.lens[i];
			}
			size_t v_size = req_data.vals.size();
			//cout << "record: "<< lens << ", val size: " << v_size << endl; // lens VS vals.size()
			CHECK_EQ(lens, req_data.vals.size());

			// Execute Push
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				size_t lens = req_data.lens[i];
				// checking values with lens CHECK_EQ(n, req_data.lens.size());
				Key key = req_data.keys[i];
				if (!store[key].size()) {
					req_key.push_back(key);
					for (size_t j = 0; j < lens; j++) {
						store[key].push_back(
								req_data.vals[tolal_lens + j] / factor); // init
					}
					ticks[skey] = current_iter;
				} else {
					for (size_t j = 0; j < lens; j++) {
						store[key][j] += (req_data.vals[tolal_lens + j]
								/ factor); //further add
					}
				}
				tolal_lens += lens;
			}
			workercount[current_iter] += 1; // For this iteration, add one
			//std::cout << "push" << ticks[skey] << " " << current_iter << " " << workercount[ticks[skey]]<< std::endl;
			if(current_iter > fast_ticks[skey]){
				fast_ticks[skey] = current_iter;
			}
			if (workercount[ticks[skey]] == num_worker) { // For a given key, if the staleness has been passed number of workers, add one
				//trigger a cb of pull
				auto& cbs = callbacks_[ticks[skey]];
				for (const auto& cb : cbs) {
					cb();
				}
				//erase anything, just release the memory
				callbacks_.erase(ticks[skey]);
				ticks[skey] += 1;
			}
			server->Response(req_meta, respush);
		} else {
			respull.keys = req_data.keys;
			/*
			 * SSP condition
			 * the slowest one + stale <= current_iter
			 * we can not pull data until
			 */
			// ticks[skey] + server->stale <= current_iter
			if (SPSSP_con_stream(callbacks_[current_iter].size(), fast_ticks[skey], server->stale, current_iter, ticks[skey])) { // Wait
				//wait for the slow workers catch up
				callbacks_[ticks[skey]].push_back(
				//callbacks_[current_iter].push_back(
						[this, req_meta, server]() mutable {
							// Check pull
							size_t n = req_key.size();
							respull.lens.resize(n);
							size_t lens = 0;
							for (size_t i = 0; i < n; ++i) {
								lens += store[req_key[i]].size();
							}
							respull.vals.resize(lens);
							// execute pull
							size_t tolal_lens = 0;
							for (size_t i = 0; i < n; ++i) {
								size_t lens = store[req_key[i]].size();
								respull.lens[i] = lens;
								for (size_t j = 0; j < lens; j++) {
									respull.vals[tolal_lens + j] = store[req_key[i]][j];
								}
								tolal_lens += lens;
							}
							server->Response(req_meta, respull);
						});
				return;
			}
			// Check pull
			respull.lens.resize(n);
			size_t lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				lens += store[key].size();
			}
			respull.vals.resize(lens);
			// execute pull
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				Key key = req_data.keys[i];
				size_t lens = store[key].size();
				respull.lens[i] = lens;
				for (size_t j = 0; j < lens; j++) {
					respull.vals[tolal_lens + j] = store[key][j];
				}
				tolal_lens += lens;
			}
			server->Response(req_meta, respull);
		}
	}
public:
	unordered_map<Key, std::vector<Val>> store;
	unordered_map<Key, Staleness> ticks;
	unordered_map<Key, Staleness> fast_ticks;
	unordered_map<Staleness, int> workercount;
	std::vector<Key> req_key;
	KVPairs<Val> respull;
	KVPairs<Val> respush;
	unordered_map<Staleness, std::vector<Callback>> callbacks_;
};


void StartServer(int argc, char *argv[]) {
	srand((int) time(0));
	if (!IsServer()) {
		return;
	}
	cout << "num of workers[" << NumWorkers() << "]" << endl;
	cout << "num of servers[" << NumServers() << "]" << endl;
#ifdef HAVE_CAFFE
	auto server = new KVServer<float>(0);
#else
	auto server = new KVServer<float>(0);
#endif

#ifdef HAVE_CAFFE
	if (!strcmp("PSSP", argv[1])) {
		server->set_request_handle(KVServerPSSPHandle_Caffe_Simple<float>());
		cout << "model : PSSP,";
		if(argc > 3) {
			Pro = atof(argv[3]);
			if (Pro > 1.0){
				Pro = 1.0/Pro;
			}
		}
		cout << " Pro : " << Pro;
	} else {
		server->set_request_handle(KVServerSSPHandle_Caffe_Simple<float>());
		cout << "model : SSP, ";
	}
	server->stale = atoi(argv[2]);
	cout << " staleness : " << server->stale << endl;
#else
	server->set_request_handle(KVServerSSPHandle<float>());
#endif
	RegisterExitCallback([server]() {delete server;});
}

void RunWorker() {
	if (!IsWorker())
		return;
	KVWorker<float> kv(0, 0);

	// init
	int num = 10000;
	vector<Key> keys(num);
	vector<float> vals(num);

	int rank = MyRank();
	srand(rank + 7);
	for (int i = 0; i < num; ++i) {
		keys[i] = kMaxKey / num * i; // kMaxKey / num * i + rank, two worker should update the same key
		vals[i] = i; //(rand() % 1000);
	}

	// push
	int repeat = 50;
	vector<int> ts;
	vector<float> rets;
	for (int i = 0; i < repeat; ++i) {
		cout << "enter iteration: " << i << endl;
		//SSP
		ts.push_back(kv.sPush(keys, vals, i));
		int w = kv.sPull(keys, &rets, i);
		std::cout << "push wait pull";
		kv.Wait(w);
	}
	//sync the rest
	for (int t : ts)
		kv.Wait(t);

	// pull
	kv.Wait(kv.Pull(keys, &rets));

	float res = 0;
	for (int i = 0; i < num; ++i) {
		res += fabs(rets[i] - vals[i] * repeat * NumWorkers()); //
	}
	CHECK_LT(res / repeat, 1e-5);
	LL<< "error: " << res / repeat;
}

void RunWorker_Caffe(int argc, char *argv[]) {
	if (!IsWorker())
		return;
	KVWorker<float> kv(0, 0);

	// init
	int num = 10000;
	vector<Key> keys(1, 1);
	vector<float> vals(num);
	vector<int> len(1, num);

	int rank = MyRank();
	srand(rank + 7);
	for (int i = 0; i < num; ++i) {
		//vals.push_back(i);
		vals[i] = i;
	}

	// push
	int repeat = 50;
	vector<int> ts;
	vector<float> rets;
	std::vector<int> ret_len;
	for (int i = 0; i < repeat; ++i) {
		cout << "caffe enter iteration: " << i << endl;
		//SSP
		ts.push_back(kv.sPush(keys, vals, i, len));
		kv.Wait(kv.sPull(keys, &rets, i, &ret_len));
	}
	//sync the rest
	for (int t : ts)
		kv.Wait(t);

	// pull
	kv.Wait(kv.Pull(keys, &rets, &ret_len));

	double res = 0;
	for (int i = 0; i < num; ++i) {
		res += fabs(rets[i] - vals[i] * repeat);
	}
	if (argc <= 1) {
		CHECK_LT(res / repeat, 1e-5);
	}
	LL<< "error: " << res / repeat;
}

int main(int argc, char *argv[]) {
	// start system
	Start(0, 5000);
	// setup server nodes
	StartServer(argc, argv);
	// run worker nodes
	//RunWorker();
#ifdef HAVE_CAFFE
	RunWorker_Caffe(argc, argv);
#else
	RunWorker();
#endif
	// stop system
	Finalize(0, true);
	return 0;
}
