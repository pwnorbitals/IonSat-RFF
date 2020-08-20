#pragma once

#if RFF_OS == FREERTOS
#include "FreeRTOS.h"
#include "FreeRTOS/RFF_FreeRTOS.h"
#else
#include "stl/stl.h"
#endif
