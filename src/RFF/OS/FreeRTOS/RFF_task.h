#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "eventHandler/uuid.h"
#include <cassert>

// #include <boost/container/static_vector.hpp>
#include <algorithm>

namespace RFF {

	/**
		\brief Thread implementation
	*/
	template<uint32_t prio = 0, uint32_t stackDepth = 2*configMINIMAL_STACK_SIZE>
	class Task {
        
        using fct_t = void(*)(void*);

	public:

		TaskHandle_t taskHandle;
		StaticTask_t task;
		StackType_t stackBuffer[stackDepth];


		// NO COPY AND NO MOVE, FREERTOS REFERENCES FIXED ADDRESSES WITHIN OBJECTS
		Task(Task const& other) = delete;
		Task& operator= (Task const& other) = delete;
		Task(Task<stackDepth>&& other) = delete;
		Task& operator= (Task&& other) = delete;
        
        bool operator==(Task const& other) { return other.taskHandle == this->taskHandle; }
		

		// TASK CREATION : https://www.freertos.org/a00019.html
		Task(fct_t _handler, std::string _name = "", void* _arg = {}) :
			task{}, stackBuffer{} {
                
                // WAS BUGGY : handler fct was std::function, went out of scope at move. Now impossible with (non-owning) function ptr
			taskHandle = xTaskCreateStatic(_handler, _name.c_str(), stackDepth, _arg, prio, stackBuffer, &task); 
			assert(taskHandle != 0);
            static_assert(prio <= configMAX_PRIORITIES);
            static_assert(stackDepth >= configMINIMAL_STACK_SIZE);
		}
		
		Task() : taskHandle{}, task{}, stackBuffer{} {};

		~Task() {
			if(taskHandle != 0) { // do not delete if moved from
				vTaskDelete(taskHandle);
			}
		}

		// TASK CONTROL : https://www.freertos.org/a00112.html
		UBaseType_t priority() {
            assert(taskHandle != 0);
			return uxTaskPriorityGet(taskHandle);
		}

		void priority(UBaseType_t uxNewPriority) {
            assert(taskHandle != 0);
			vTaskPrioritySet(taskHandle, uxNewPriority);
		}

		void suspend() {
            assert(taskHandle != 0);
			vTaskSuspend(taskHandle);
		}		

		void resume() {
            assert(taskHandle != 0);
			vTaskResume(taskHandle);
		}

		BaseType_t resumeFromISR() {
            assert(taskHandle != 0);
			return xTaskResumeFromISR(taskHandle);
		}

		BaseType_t abortDelay() {
            assert(taskHandle != 0);
			return xTaskAbortDelay(taskHandle);
		}

		// TASK UTILITIES : https://www.freertos.org/a00021.html
		// TODO


	};

	[[maybe_unused]] static void delay(const TickType_t xTicksToDelay) {
		vTaskDelay(xTicksToDelay);
	}

	[[maybe_unused]] static xTaskHandle currentHandle(void) {
		return xTaskGetCurrentTaskHandle();
	}

	[[maybe_unused]] static void delayUntil(TickType_t* pxPreviousWakeTime, const TickType_t xTimeIncrement) {
		vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
	}

	[[maybe_unused]] static void suspendCurrent() {
		vTaskSuspend(NULL);
	}

	[[maybe_unused]] static TickType_t getTickCount() {
		return xTaskGetTickCount();
	}
}
