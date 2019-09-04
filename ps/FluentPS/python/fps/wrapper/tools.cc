/*
 * tools.cc
 *
 *  Created on: 4 Jul 2019
 *      Author: xyao
 */
#include "../wrapper/tools.h"

/*
 PyObject* result = PyList_New(0);
 int i;
 for (i = 0; i < 100; ++i){
 PyList_Append(result, PyLong_FromLong(i));
 }
 return result;*/

void vectorToList_Float(PyObject* py_, const std::vector<float>* vals) {
	unsigned int len = vals->size();
	if (py_ == 0) {
		py_ = PyList_New(0);
		for (unsigned int i = 0; i < len; i++) {
			PyList_Append(py_, PyFloat_FromDouble((double) (*vals)[i]));
		}
		// std::cout << PyList_Size(py_) << std::endl;
	} else if (PyList_Size(py_) != len) {
		unsigned int old_len = PyList_Size(py_);
		for (unsigned int i = 0; i < old_len; i++) {
			PyList_SET_ITEM(py_, i, PyFloat_FromDouble((double) (*vals)[i]));
		}
		for (unsigned int i = old_len; i < len; i++) {
			PyList_Append(py_, PyFloat_FromDouble((double) (*vals)[i]));
		}
	} else {
		for (unsigned int i = 0; i < len; i++) {
			/*
			 PyObject *num = PyFloat_FromDouble((double) (*vals)[i]);
			 if (!num) {
			 Py_DECREF (py_);
			 throw std::logic_error("Unable to allocate memory for Python list");
			 }
			 PyList_SET_ITEM(py_, i, num);*/
			PyList_SET_ITEM(py_, i, PyFloat_FromDouble((double) (*vals)[i]));
		}
		py_ = PyList_GetSlice(py_, 0, len);
	}
}

// We do not need this function
void vectorToList_Int(PyObject* py_, const std::vector<ps::Key>* keys) {
	unsigned int len = keys->size();
	if (py_ == 0) {
		py_ = PyList_New(0);
		for (unsigned int i = 0; i < len; i++) {
			PyList_Append(py_,
					PyLong_FromUnsignedLong((unsigned long) (*keys)[i]));
		}
		// std::cout << PyList_Size(py_) << std::endl;
	} else if (PyList_Size(py_) != len) {
		unsigned int old_len = PyList_Size(py_);
		for (unsigned int i = 0; i < old_len; i++) {
			PyList_SET_ITEM(py_, i,
					PyLong_FromUnsignedLong((unsigned long) (*keys)[i]));
		}
		for (unsigned int i = old_len; i < len; i++) {
			PyList_Append(py_,
					PyLong_FromUnsignedLong((unsigned long) (*keys)[i]));
		}
	} else {
		for (unsigned int i = 0; i < len; i++) {
			PyList_SET_ITEM(py_, i,
					PyLong_FromUnsignedLong((unsigned long) (*keys)[i]));
		}
		py_ = PyList_GetSlice(py_, 0, len);
	}
}

void ListTovector_Float(PyObject* py_, std::vector<float>* vals) {
	if (PyList_Check(py_)) {
		unsigned int len = PyList_Size(py_);
		if (vals == 0) {
			vals = new std::vector<float>(len);
			for (Py_ssize_t i = 0; i < len; i++) {
				vals->push_back(
						((float) PyFloat_AsDouble(PyList_GetItem(py_, i))));
			}
		} else if (len != vals->size()) {
			unsigned int old_len = vals->size();
			for (Py_ssize_t i = 0; i < old_len; i++) {
				(*vals)[i] = ((float) PyFloat_AsDouble(PyList_GetItem(py_, i)));
			}
			for (Py_ssize_t i = old_len; i < len; i++) {
				vals->push_back(
						((float) PyFloat_AsDouble(PyList_GetItem(py_, i))));
			}
		} else {
			for (Py_ssize_t i = 0; i < len; i++) {
				(*vals)[i] = ((float) PyFloat_AsDouble(PyList_GetItem(py_, i)));
			}
		}
	} else {
		throw std::logic_error("Passed PyObject pointer was not a list!");
	}
}

void ListTovector_Long(PyObject* py_, std::vector<ps::Key>* keys) {
	if (PyList_Check(py_)) {
		unsigned int len = PyList_Size(py_);
		if (keys == 0) {
			keys = new std::vector<ps::Key>(len);
			for (Py_ssize_t i = 0; i < len; i++) {
				keys->push_back(
						((ps::Key) PyLong_AsUnsignedLong(PyList_GetItem(py_, i))));
			}
		} else if (len != keys->size()) {
			unsigned int old_len = keys->size();
			for (Py_ssize_t i = 0; i < old_len; i++) {
				(*keys)[i] = ((ps::Key) PyLong_AsUnsignedLong(
						PyList_GetItem(py_, i)));
			}
			for (Py_ssize_t i = old_len; i < len; i++) {
				keys->push_back(
						((ps::Key) PyLong_AsUnsignedLong(PyList_GetItem(py_, i))));
			}
		} else {
			for (Py_ssize_t i = 0; i < len; i++) {
				/*
				 PyObject *value = PyList_GetItem(py_, i);
				 keys->push_back(((ps::Key)PyLong_AsUnsignedLongLong(value)));*/
				(*keys)[i] = ((ps::Key) PyLong_AsUnsignedLong(
						PyList_GetItem(py_, i)));
			}
		}
	} else {
		throw std::logic_error("Passed PyObject pointer was not a list!");
	}
}

void ListTovector_uInt(PyObject* py_, std::vector<int>* lens) {
	if (PyList_Check(py_)) {
		unsigned int len = PyList_Size(py_);
		if (lens == 0) {
			lens = new std::vector<int>(len);
			for (Py_ssize_t i = 0; i < len; i++) {
				lens->push_back(((int) PyLong_AsLong(PyList_GetItem(py_, i))));
			}
		} else if (len != lens->size()) {
			unsigned int old_len = lens->size();
			for (Py_ssize_t i = 0; i < old_len; i++) {
				(*lens)[i] = ((int) PyLong_AsLong(PyList_GetItem(py_, i)));
			}
			for (Py_ssize_t i = old_len; i < len; i++) {
				lens->push_back(((int) PyLong_AsLong(PyList_GetItem(py_, i))));
			}
		} else {
			for (Py_ssize_t i = 0; i < len; i++) {
				/*
				 PyObject *value = PyList_GetItem(py_, i);
				 keys->push_back(((ps::Key)PyLong_AsUnsignedLongLong(value)));*/
				(*lens)[i] = ((int) PyLong_AsLong(PyList_GetItem(py_, i)));
			}
		}
	} else {
		throw std::logic_error("Passed PyObject pointer was not a list!");
	}
}

void createList(PyObject* py_) {
	int i;
	for (i = 0; i < 100; ++i) {
		PyList_Append(py_, PyLong_FromLong(i));
	}
}

extern "C" {

int NumWorkers() {
	return ps::NumWorkers();
}
/** \brief Returns the number of server nodes */
int NumServers() {
	return ps::NumServers();
}
/** \brief Returns true if this node is a worker node */
bool IsWorker() {
	return ps::IsWorker();
}
/** \brief Returns true if this node is a server node. */
bool IsServer() {
	return ps::IsServer();
}
/** \brief Returns true if this node is a scheduler node. */
bool IsScheduler() {
	return ps::IsScheduler();
}

PyObject* Test_array_1(PyObject* in_) {
	std::vector<ps::Key>* tmp;
	PyObject* result = 0;
	if (tmp == 0) {
		tmp = new std::vector<unsigned long>(PyList_Size(in_));
	}
	ListTovector_Long(in_, tmp);
	if (result == 0) {
		result = PyList_New(0);
	}
	vectorToList_Int(result, tmp);
	return result;
}

PyObject* Test_array_2() {
	PyObject* result = 0;
	std::vector<float>* s = new std::vector<float>(100);
	for (int i = 0; i < 100; ++i) {
		(*s)[i] = (float) i;
	}
	if (result == 0) {
		result = PyList_New(0);
	}
	vectorToList_Float(result, s);
	return result;
}

PyObject* Test_array_3() {
	PyObject* result = 0;
	result = PyList_New(0);
	createList(result);
	return result;
}

}
