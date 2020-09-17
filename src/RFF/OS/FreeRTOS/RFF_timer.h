#pragma once

#include "FreeRTOS.h"

namespace RFF {
    #if configUSE_TIMERS 
    template<int count>
    class Timer {
            StaticTimer_t buffer;
            TimerHandle_t handle;
            void (*cb) (void*);

            Timer(TickType_t period, bool autoReload, void (*_cb)(void*)) 
                : buffer{}, 
                  cb{_cb}, 
                  handle{xTimerCreateStatic("", period, autoReload, 0, Timer<count>::callback, buffer)}
                {
                assert(cb != nullptr);
                assert(handle != NULL);
            }

            

            static void callback( TimerHandle_t xTimer ) {
                cb();
            }


        public:

            ~Timer () {
                xTimerDelete( handle, portMAX_DELAY );
            }
            
            static Timer& instance(TickType_t period = 0, bool autoReload = true, void (*cb) (void*) = nullptr) {
                static Timer<count> instance{period, autoReload, cb};
                return instance;
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

            bool startFromISR() {
                return xTimerStartFromISR(handle, NULL);
            }

            bool stopFromISR() {
                return xTimerStopFromISR(handle, NULL);
            }


            bool changePeriodFromISR(TickType_t newPeriod) {
                return xTimerChangePeriodFromISR(handle, newPeriod, NULL);
            }

            bool resetFromISR() {
                return xTimerResetFromISR(handle, portMAX_DELAY);
            }

            // TODO : setTimerID
            // TODO : getTimerDaemonTaskHandle
            // TODO : PendFunctionCall
            // TODO : PendFunctionCallFromISR
            // TODO : getPeriod
            // TODO : getExpiryTime
            








    };


    #endif
}