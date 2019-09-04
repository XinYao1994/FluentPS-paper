/*
 * server.h
 *
 *  Created on: 5 Jul 2019
 *      Author: xyao
 */

#ifndef PS_FLUENTPS_PYTHON_FPS_WRAPPER_SERVER_H_
#define PS_FLUENTPS_PYTHON_FPS_WRAPPER_SERVER_H_

#include "ps/ps.h"
#include <unistd.h>
#include <math.h>
#include <iostream>

#define PrintSyc

using Staleness = uint64_t;
using Callback = std::function<void()>;
double Pro = 1.0;

// ps::KVWorker<char>* BytePSGlobal::_ps = NULL;
ps::KVServer<float> *server;

extern "C" {

// create a init process
void Init_s(int custom_id, int timeout);

void create_s(int app_id);

void start_s(int staleness, double _Pro);

//create a finalize operation
void Finalize_s(int custom_id);

}



#endif /* PS_FLUENTPS_PYTHON_FPS_WRAPPER_SERVER_H_ */
