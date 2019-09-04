/*
 * scheduler.h
 *
 *  Created on: 5 Jul 2019
 *      Author: xyao
 */

#ifndef PS_FLUENTPS_PYTHON_FPS_WRAPPER_SCHEDULER_H_
#define PS_FLUENTPS_PYTHON_FPS_WRAPPER_SCHEDULER_H_

#include "ps/ps.h"


#define DEBUG
// ps::KVWorker<char>* BytePSGlobal::_ps = NULL;

extern "C" {

// create a init process
void Init(int custom_id, int timeout);

//create a finalize operation
void Finalize(int custom_id);

}



#endif /* PS_FLUENTPS_PYTHON_FPS_WRAPPER_SCHEDULER_H_ */
