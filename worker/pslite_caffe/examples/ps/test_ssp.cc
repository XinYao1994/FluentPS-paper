#include "ps/ps.h"
#include <unistd.h>
using namespace ps;
using namespace std;

#define HAVE_CAFFE

void StartServer() {
	if (!IsServer()) {
		return;
	}
	cout << "num of servers[" << NumServers() << "]" << endl;
	auto server = new KVServer<float>(0);
	server->set_request_handle(KVServerDefaultHandle<float>());
	RegisterExitCallback([server]() {delete server;});
}

//KVWorker<float> *kv;
int num_w = 0;
vector<SimpleData> buffer;

void SchedulerReqHandle(const SimpleData& req, SimpleApp* app) {
	num_w++;
	if (num_w == NumWorkers()) {
		num_w = 0;
		for (auto const& req : buffer) {
			app->Response(req);
		}
		buffer.clear();
		app->Response(req);
	} else {
		buffer.push_back(req);
	}
	//app->Response(req);
}

void ASP_SchedulerReqHandle(const SimpleData& req, SimpleApp* app) {
	app->Response(req);
}

void StartScheduler(int argc, char *argv[]) {
	if (!IsScheduler()) {
		return;
	}
	KVWorker<float> *kv = new KVWorker<float>(0, 0);
	//app->set_request_handle(SchedulerReqHandle); //SchedulerReqHandle()
	if (!strcmp("ASP", argv[1])) {
		kv->set_request_handle(ASP_SchedulerReqHandle);
	} else {
		kv->set_request_handle(SchedulerReqHandle);
	}
}

void RunWorker_Caffe() {
	if (!IsWorker())
		return;
	KVWorker<float> *kv = new KVWorker<float>(0, 0);
	//KVWorker<float> kv(0, 0);
	// init
	int num = 100000000;
	vector<Key> keys(num);
	vector<float> vals(num);

	int rank = MyRank();
	srand(rank + 7);
	for (int i = 0; i < num; ++i) {
		keys[i] = i; // kMaxKey / num * i + rank, two worker should update the same key
		vals[i] = i; //(rand() % 1000);
	}
	// push
	int repeat = 50;
	vector<int> ts;
	vector<float> rets;
	for (int i = 0; i < repeat; ++i) {
		//BSP
		//kv.Wait(kv.Push(keys, vals));
		kv->Wait(kv->Push(keys, vals));
		//kv.Push(keys, vals);
		//cout << "send to scheduler: " << i << endl;
		//send to schduler for sync
		//kv.Wait(kv.Request(1, "", kScheduler));
		kv->Wait(kv->Request(1, "", kScheduler));
		//app->Wait(app->Request(1, "", kScheduler));
		cout << "enter iteration: " << i << endl;

		//kv.Wait(kv.Pull(keys, &rets));
		kv->Wait(kv->Pull(keys, &rets));

		//ASP
		//ts.push_back(kv.Push(keys, vals));
		//kv.Wait(kv.Pull(keys, &rets));

	}
	LL<< "Done ! ";
	LL<< rets[0];
}

int main(int argc, char *argv[]) {
	// start system
	Start(0);
	// setup server nodes
	/*
	 if(!IsServer()){
	 kv = new KVWorker<float>(0, 0);
	 }*/
	StartScheduler(argc, argv);
	// setup server nodes
	StartServer();
	// run worker nodes
	//RunWorker();
#ifdef HAVE_CAFFE
	RunWorker_Caffe();
#else

#endif
	// stop system
	Finalize(0, true);
	return 0;
}
