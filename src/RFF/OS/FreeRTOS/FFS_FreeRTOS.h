#pragma once

#include <functional>

#include <iostream>

#include "FFS_task.h"
#include "FFS_queue.h"
#include "FFS_semaphore.h"
#include "FFS_queueSet.h"

static FFS::Task<INIT_TASK_STACK> initTask;
void ffs_main(void* empty);

namespace FFS {

	struct OSSettings {

	};

	void OSStart();
	void OSStop();
    
    FFS::Task<INIT_TASK_STACK> const& getInitTask();



}
