#include "RFF.h"

#ifdef RFF_USE_CATCH
	#define CATCH_CONFIG_RUNNER
	#include "catch.hpp"
#endif

extern void rff_main();

void user_main(void*) {
	rff_main();
	RFF::suspendCurrent();
}

namespace RFF {
    Controller* ctrlr = nullptr;
    
	void OSStart() {
		vTaskStartScheduler();
	}

	void OSStop() {
		vTaskEndScheduler();
	}
	
}

extern "C" {
    int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
		auto result = 0;
		#ifdef RFF_USE_CATCH
			result = Catch::Session().run( argc, argv );
		#endif
                
        auto task = RFF::Task<configMAX_PRIORITIES - 1>(user_main, "init");
        RFF::OSStart();
        
        return result;
    }

    void hard_fault_handler (void) {
        while(1){};
    }
    
        

	void vMainQueueSendPassed(void) {
		/* This is just an example implementation of the "queue send" trace hook. */

	}

	void vApplicationIdleHook(void) {

#ifdef __GCC_POSIX__
		struct timespec xTimeToSleep, xTimeSlept;
		/* Makes the process more agreeable when using the Posix simulator. */
		xTimeToSleep.tv_sec = 1;
		xTimeToSleep.tv_nsec = 0;
		nanosleep(&xTimeToSleep, &xTimeSlept);
#endif
	}


// From https://www.freertos.org/a00110.html
	/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
	implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
	used by the Idle task. */
	void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
	                                   StackType_t** ppxIdleTaskStackBuffer,
	                                   uint32_t* pulIdleTaskStackSize) {
		/* If the buffers to be provided to the Idle task are declared inside this
		function then they must be declared static – otherwise they will be allocated on
		the stack and so not exists after this function exits. */
		static StaticTask_t xIdleTaskTCB;
		static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

		/* Pass out a pointer to the StaticTask_t structure in which the Idle task’s
		state will be stored. */
		*ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

		/* Pass out the array that will be used as the Idle task’s stack. */
		*ppxIdleTaskStackBuffer = uxIdleTaskStack;

		/* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
		Note that, as the array is necessarily of type StackType_t,
		configMINIMAL_STACK_SIZE is specified in words, not bytes. */
		*pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
	}
	/*———————————————————–*/

	/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
	application must provide an implementation of vApplicationGetTimerTaskMemory()
	to provide the memory that is used by the Timer service task. */
	void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
	                                    StackType_t** ppxTimerTaskStackBuffer,
	                                    uint32_t* pulTimerTaskStackSize) {
		/* If the buffers to be provided to the Timer task are declared inside this
		function then they must be declared static – otherwise they will be allocated on
		the stack and so not exists after this function exits. */
		static StaticTask_t xTimerTaskTCB;
		static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

		/* Pass out a pointer to the StaticTask_t structure in which the Timer
		task’s state will be stored. */
		*ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

		/* Pass out the array that will be used as the Timer task’s stack. */
		*ppxTimerTaskStackBuffer = uxTimerTaskStack;

		/* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
		Note that, as the array is necessarily of type StackType_t,
		configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
		*pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
	}


	// https://www.freertos.org/pvTaskGetThreadLocalStoragePointer.html
	/*
	inline void* pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery, BaseType_t xIndex ){
	    // TODO
	}

	inline void vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,  BaseType_t xIndex, void *pvValue ){
	    // TODO

	inline TaskHandle_t xTaskGetIdleTaskHandle() {

	}
	*/
}

