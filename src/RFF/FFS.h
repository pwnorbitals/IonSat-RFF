#ifndef FFS_HPP_INC
#define FFS_HPP_INC

#include <iostream>

#define OS FREERTOS

#include "../config/config.h"
#include "OS/OS.h"
#include "module/module.h"
#include "controller/controller.h"
#include "mode/mode.h"
#include "event/event.h"
#include "eventHandler/eventHandler.h"

namespace FFS {
	void iotest();
}

#endif
