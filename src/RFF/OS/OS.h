#pragma once

#if FFS_OS == FREERTOS
#include "FreeRTOS.h"
#include "FreeRTOS/FFS_FreeRTOS.h"
#else
#include "stl/stl.h"
#endif
