#pragma once

#include "FreeRTOS.h"

namespace RFF {
    class Timer {
            StaticTimer_t buffer;
            TimerHandle_t handle;
        public:
            void callback( TimerHandle_t xTimer ) {

            }

            Timer(TickType_t period, bool autoReload) : buffer{}, handle{
                xTimerCreateStatic("", period, autoReload, 0, callback, buffer)
            }{
                assert(handle != NULL);
            }
    };
}