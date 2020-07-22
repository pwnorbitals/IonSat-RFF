#pragma once

#include <functional>

#include <iostream>

#include "event/event.h"
#include "FFS_task.h"
#include "FFS_queue.h"
#include "FFS_semaphore.h"

void ffs_main(void* empty);

namespace FFS {

	struct OSSettings {

	};

	void OSStart();
	void OSStop();



}
