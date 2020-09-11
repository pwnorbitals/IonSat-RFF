#pragma once

#include "FreeRTOS.h"

namespace RFF {
    #if configUSE_TIMERS 
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

            ~Timer () {
                xTimerDelete( handle, portMAX_DELAY );
            }

            bool isActive() {

            }
          
            void* getId() {
                return pvTimerGetTimerID(handle);
            }

            void setReloadMode(bool const autoReload) {
                vTimerSetReloadMode(autoReload);
            }

            bool start() {
                return xTimerStart(handle, portMAX_DELAY);
            }

            bool stop() {
                return xTimerStop(handle, portMAX_DELAY);
            }

            bool changePeriod(TickType_t newPeriod) {
                return xTimerChangePeriod(handle, newPeriod, portMAX_DELAY);
            }

            bool reset () {
                return xTimerReset(handle, portMAX_DELAY);
            }

            





    };


    #endif
}