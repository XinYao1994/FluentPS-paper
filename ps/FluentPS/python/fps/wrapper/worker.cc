#include "../wrapper/worker.h"

void EvenSlicer(const ps::KVPairs<float>& send,
		const std::vector<ps::Range>& ranges,
		typename ps::KVWorker<float>::SlicedKVs* sliced) {
	sliced->resize(ranges.size());

	// find the positions in msg.key
	size_t n = ranges.size();
	std::vector < size_t > pos(n + 1);
	const ps::Key* begin = send.keys.begin();
	const ps::Key* end = send.keys.end();
#ifdef SLICE_DEBUG
  for(auto it=begin;it!=end;++it) std::cout << *it << std::endl;
  for(auto it=send.keys.begin();it!=send.keys.end(); ++it){
 	  ps::Key tmp = *it;
 	  std::cout << tmp << std::endl;
   }
#endif
	for (size_t i = 0; i < n; ++i) {
		if (i == 0) {
			pos[0] = std::lower_bound(begin, end, ranges[0].begin()) - begin;
			begin += pos[0];
		} else {
			CHECK_EQ(ranges[i - 1].end(), ranges[i].begin());
		}
		size_t len = std::lower_bound(begin, end, ranges[i].end()) - begin;
		begin += len;
		pos[i + 1] = pos[i] + len;

		// don't send it to servers for empty kv
		sliced->at(i).first = (len != 0);
	}
	CHECK_EQ(pos[n], send.keys.size());
	if (send.keys.empty())
		return;

	// the length of value
	size_t k = 0, val_begin = 0, val_end = 0;
	if (send.lens.empty()) {
		k = send.vals.size() / send.keys.size();
		CHECK_EQ(k * send.keys.size(), send.vals.size());
	} else {
		CHECK_EQ(send.keys.size(), send.lens.size());
	}

	// slice
	for (size_t i = 0; i < n; ++i) {
		if (pos[i + 1] == pos[i]) {
			sliced->at(i).first = false;
			continue;
		}
		sliced->at(i).first = true;
		auto& kv = sliced->at(i).second;
		kv.keys = send.keys.segment(pos[i], pos[i + 1]);
		if (send.lens.size()) {
			kv.lens = send.lens.segment(pos[i], pos[i + 1]);
			for (int l : kv.lens)
				val_end += l;
			kv.vals = send.vals.segment(val_begin, val_end);
			val_begin = val_end;
		} else {
			kv.vals = send.vals.segment(pos[i] * k, pos[i + 1] * k);
		}
	}
}

extern "C" {

//void

// create a init process
void Init_w(int custom_id, int timeout = 5) {
	ps::Start(custom_id, timeout);
}

void create_w(int app_id, int custom_id) {
	if (worker) {
		return;
	}
	worker = new ps::KVWorker<float>(app_id, custom_id);
}

void start_w() {
	if (!worker) {
		return;
	}
	worker->set_slicer(EvenSlicer);
}

int push(PyObject* py_keys, PyObject* py_vals, PyObject* py_len) {
	if(global_keys==0){
		global_keys = new std::vector<ps::Key>(1);
	}
	if(global_gradient==0){
		global_gradient = new std::vector<float>(1);
	}
	if(PyList_Check(py_len)){
		if(global_lens==0){
			global_lens = new std::vector<int>(1);
		}
		ListTovector_uInt(py_len, global_lens);
	}
	ListTovector_Long(py_keys, global_keys);
	ListTovector_Float(py_vals, global_gradient);
	return worker->Push(*global_keys, *global_gradient, *global_lens); //
}

int pull(PyObject* py_keys, PyObject* py_len) {
	if(global_keys==0){
		global_keys = new std::vector<ps::Key>(1);
	}
	if(global_vals==0){
		global_vals = new std::vector<float>();
	}
	if(PyList_Check(py_len)){
		if(global_lens==0){
			global_lens = new std::vector<int>(1);
		}
		ListTovector_uInt(py_len, global_lens);
	}
	ListTovector_Long(py_keys, global_keys);
	return worker->Pull(*global_keys, global_vals); //, *global_lens
}

void wait(int timestamp) {
	worker->Wait(timestamp);
}

int spush(PyObject* py_keys, PyObject* py_vals, int staleness, PyObject* py_len) {
	if(global_keys==0){
		global_keys = new std::vector<ps::Key>(1);
	}
	if(global_gradient==0){
		global_gradient = new std::vector<float>(1);
	}
	if(PyList_Check(py_len)){
		if(global_lens==0){
			global_lens = new std::vector<int>(1);
		}
		ListTovector_uInt(py_len, global_lens);
	}
	ListTovector_Long(py_keys, global_keys);
	ListTovector_Float(py_vals, global_gradient);
	return worker->sPush(*global_keys, *global_gradient, staleness, *global_lens);
}

int spull(PyObject* py_keys, int staleness, PyObject* py_len) {
	if(global_keys==0){
		global_keys = new std::vector<ps::Key>(1);
	}
	if(global_vals==0){
		global_vals = new std::vector<float>();
	}
	if(PyList_Check(py_len)){
		if(global_lens==0){
			global_lens = new std::vector<int>(1);
		}
		ListTovector_uInt(py_len, global_lens);
	}
	ListTovector_Long(py_keys, global_keys);
	return worker->sPull(*global_keys, global_vals, staleness); //, *global_lens
}

PyObject* read_vals() {
	if(py_global_vals == 0){
		py_global_vals = PyList_New(0);
	}
	if(global_vals == 0){
		return nullptr;
	}
	vectorToList_Float(py_global_vals, global_vals);
	return py_global_vals;
}

//create a finalize operation
void Finalize_w(int custom_id) {
	ps::Finalize(custom_id, true);
}
}
