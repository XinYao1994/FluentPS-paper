/*
 * scheduler.cc
 *
 *  Created on: 5 Jul 2019
 *      Author: xyao
 */

#include "../wrapper/scheduler.h"
// ps::KVWorker<char>* BytePSGlobal::_ps = NULL;
extern "C" {

// create a init process
void Init(int custom_id, int timeout=5){
	ps::Start(custom_id, timeout);
}

//create a finalize operation
void Finalize(int custom_id){
	ps::Finalize(custom_id, true);
}

}


