/*
 * tools.h
 *
 *  Created on: 4 Jul 2019
 *      Author: xyao
 */

#ifndef PS_FLUENTPS_PYTHON_FPS_WRAPPER_TOOLS_H_
#define PS_FLUENTPS_PYTHON_FPS_WRAPPER_TOOLS_H_

#include "ps/ps.h"
#include <vector>
#include <Python.h>
#include <stdexcept>

#define DEBUG
// ps::KVWorker<char>* BytePSGlobal::_ps = NULL;
void vectorToList_Float(PyObject* py_, const std::vector<float>* vals);

// We do not need this function
void vectorToList_Int(PyObject* py_, const std::vector<ps::Key>* keys);

void ListTovector_Float(PyObject* py_, std::vector<float>* vals);

void ListTovector_Long(PyObject* py_, std::vector<ps::Key>* keys);

void ListTovector_uInt(PyObject* py_, std::vector<int>* lens);

extern "C" {

int NumWorkers();
/** \brief Returns the number of server nodes */
int NumServers();
/** \brief Returns true if this node is a worker node */
bool IsWorker();
/** \brief Returns true if this node is a server node. */
bool IsServer();
/** \brief Returns true if this node is a scheduler node. */
bool IsScheduler();

PyObject* Test_array_1(PyObject* in_);
PyObject* Test_array_2();
PyObject* Test_array_3();


}




#endif /* PS_FLUENTPS_PYTHON_FPS_WRAPPER_TOOLS_H_ */
