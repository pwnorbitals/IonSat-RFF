#pragma once

#include "FreeRTOS.h"
#include "timers.h"


// FROM https://stackoverflow.com/a/6174263/3753446
#define COUNTER_READ_CRUMB( TAG, RANK, ACC ) counter_crumb( TAG(), constant_index< RANK >(), constant_index< ACC >() )
#define COUNTER_READ( TAG ) COUNTER_READ_CRUMB( TAG, 1, COUNTER_READ_CRUMB( TAG, 2, COUNTER_READ_CRUMB( TAG, 4, COUNTER_READ_CRUMB( TAG, 8, \
    COUNTER_READ_CRUMB( TAG, 16, COUNTER_READ_CRUMB( TAG, 32, COUNTER_READ_CRUMB( TAG, 64, COUNTER_READ_CRUMB( TAG, 128, 0 ) ) ) ) ) ) ) )

#define COUNTER_INC( TAG ) \
constexpr \
constant_index< COUNTER_READ( TAG ) + 1 > \
counter_crumb( TAG, constant_index< ( COUNTER_READ( TAG ) + 1 ) & ~ COUNTER_READ( TAG ) >, \
                                                constant_index< ( COUNTER_READ( TAG ) + 1 ) & COUNTER_READ( TAG ) > ) { return {}; }

#define COUNTER_LINK_NAMESPACE( NS ) using NS::counter_crumb;

template< std::size_t n >
struct constant_index : std::integral_constant< std::size_t, n > {};

template< typename id, std::size_t rank, std::size_t acc >
constexpr constant_index< acc > counter_crumb( id, constant_index< rank >, constant_index< acc > ) { return {}; } // found by ADL via constant_index


struct timer_cnt {};





namespace RFF {
    #if configUSE_TIMERS 

    template<int count>
    class Timer {
            StaticTimer_t buffer;
            TimerHandle_t handle;
            void (*cb) ();

            Timer(TickType_t period, bool autoReload, void (*_cb)()) 
                : buffer{}, 
                  cb{_cb}, 
                  handle{xTimerCreateStatic("", period, autoReload, 0, Timer<count>::instance().callback, &buffer)}
                {
                assert(cb != nullptr);
                assert(handle != NULL);
            }

            

            


        public:

            void callback( TimerHandle_t xTimer ) {
                // We may want to do some additional stuff ?
                cb();
            }

            ~Timer () {
                xTimerDelete( handle, portMAX_DELAY );
            }
            
            static Timer& instance(TickType_t period = 0, bool autoReload = true, void (*cb) () = nullptr) {
                static Timer<count> instance{period, autoReload, cb};
                return instance;
            }
            

            bool isActive() {
                return xTimerIsTimerActive(handle);
            }
          
            void* getId() {
                return pvTimerGetTimerID(handle);
            }

            void setReloadMode(bool const autoReload) {
                vTimerSetReloadMode(handle, autoReload);
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

    template<typename T>
    T& InitTimer(TickType_t period, bool autoReload, void (*cb)()) {
        COUNTER_INC( timer_cnt );
        int constexpr cnt = COUNTER_READ( timer_cnt );
        return RFF::Timer<cnt>::instance(period, autoReload, cb);
    }


    #endif
}