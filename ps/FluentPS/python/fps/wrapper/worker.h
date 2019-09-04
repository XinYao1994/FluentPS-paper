/*
 * worker.h
 *
 *  Created on: 5 Jul 2019
 *      Author: xyao
 */

#ifndef PS_FLUENTPS_PYTHON_FPS_WRAPPER_WORKER_H_
#define PS_FLUENTPS_PYTHON_FPS_WRAPPER_WORKER_H_


#include "ps/ps.h"
#include "../wrapper/tools.h"

#define DEBUG
// ps::KVWorker<char>* BytePSGlobal::_ps = NULL;
ps::KVWorker<float> *worker;
std::vector<ps::Key>* global_keys = 0;
std::vector<float>* global_gradient = 0;
std::vector<float>* global_vals = 0;
std::vector<int>* global_lens = 0;
PyObject* py_global_vals = 0;

extern "C" {

// create a init process
void Init_w(int custom_id, int timeout);

void create_w(int app_id, int custom_id);

void start_w();

int push(PyObject* py_keys, PyObject* py_vals, PyObject* py_len);

int pull(PyObject* py_keys, PyObject* py_len);

void wait(int timestamp);

int spush(PyObject* py_keys, PyObject* py_vals, int staleness, PyObject* py_len);

int spull(PyObject* py_keys, int staleness, PyObject* py_len);

PyObject* read_vals();

//create a finalize operation
void Finalize_w(int custom_id);

}



#endif /* PS_FLUENTPS_PYTHON_FPS_WRAPPER_WORKER_H_ */
