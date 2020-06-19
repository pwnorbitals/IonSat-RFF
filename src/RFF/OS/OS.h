#ifndef RTOS_H_INC
#define RTOS_H_INC

#define OS FREERTOS

#if OS == FREERTOS
    #include "FreeRTOS/FFS_FreeRTOS.h"
#else
    #include "stl/stl.h"
#endif

#endif
