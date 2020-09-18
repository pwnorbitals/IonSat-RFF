#pragma once

#include <functional>

#include <iostream>

#include "RFF_task.h"
#include "RFF_queue.h"
#include "RFF_semaphore.h"
#include "RFF_queueSet.h"
#include "RFF_timer.h"

static RFF::Task<INIT_TASK_STACK> initTask;
void RFF_main(void* empty);

namespace RFF {

	void OSStart();
	void OSStop();
    
    RFF::Task<INIT_TASK_STACK> const& getInitTask();



}

extern "C" {
    void hard_fault_handler(void);
}
