#include "../wrapper/server.h"


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

template<class Val>
class KVServerPSSPHandle_Simple {
public:
	/**
	 *req_meta: the meta info about this request, including cmd push sender timestamp customer_id
	 req_data: the data, key, value, value lens
	 server: the pointer of the current PS
	 */
	void operator()(const ps::KVMeta& req_meta, const ps::KVPairs<Val>& req_data,
			ps::KVServer<Val>* server) {
		size_t n = req_data.keys.size();
		int num_worker = ps::NumWorkers();
		double factor = (double) num_worker;
		int current_iter = req_meta.staleness;
		ps::Key skey = req_data.keys[0];
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
				ps::Key key = req_data.keys[i];
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
				ps::Key key = req_data.keys[i];
				lens += store[key].size();
			}
			respull.vals.resize(lens);
			// execute pull
			size_t tolal_lens = 0;
			for (size_t i = 0; i < n; ++i) {
				ps::Key key = req_data.keys[i];
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
	std::unordered_map<ps::Key, std::vector<Val>> store;
	std::unordered_map<ps::Key, Staleness> ticks;
	std::unordered_map<ps::Key, Staleness> fast_ticks;
	std::unordered_map<Staleness, int> workercount;
	std::vector<ps::Key> req_key;
	ps::KVPairs<Val> respull;
	ps::KVPairs<Val> respush;
	std::unordered_map<Staleness, std::vector<Callback>> callbacks_;
};


extern "C" {
// create a init process
void Init_s(int custom_id, int timeout=5){
	ps::Start(custom_id, timeout);
}

void create_s(int app_id){
	if(server){
		return ;
	}
	std::cout << "num of workers[" << ps::NumWorkers() << "]" << std::endl;
	std::cout << "num of servers[" << ps::NumServers() << "]" << std::endl;
	server = new ps::KVServer<float>(app_id);
}

void start_s(int staleness=0, double _Pro=1.0){
	if(!server){
		return ;
	}
	server->stale = staleness;
	Pro = _Pro;
	server->set_request_handle(KVServerPSSPHandle_Simple<float>());
	ps::RegisterExitCallback([server]() {delete server;});
}

//create a finalize operation
void Finalize_s(int custom_id){
	ps::Finalize(custom_id, true);
}

}
