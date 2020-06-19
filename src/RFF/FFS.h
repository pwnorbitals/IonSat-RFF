#ifndef FFS_HPP_INC
#define FFS_HPP_INC

#include <iostream>

#define OS FREERTOS

#include "OS/OS.h"
#include "module/module.h"
#include "controller/controller.h"
#include "mode/mode.h"


namespace FFS {
    void iotest();
}

#endif
