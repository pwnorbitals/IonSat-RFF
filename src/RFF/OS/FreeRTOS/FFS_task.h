#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <boost/container/static_vector.hpp>
#include <algorithm>

namespace FFS {
	template<uint32_t stackDepth>
	class Task {
        
        using me_t = Task<stackDepth>;
        using fct_t = void(*)(void*);

	public:

		TaskHandle_t taskHandle;
		StaticTask_t task;
		StackType_t stackBuffer[stackDepth];


		// NO COPY
		Task(Task const& other) = delete;
		Task& operator= (Task const& other) = delete;
        
        
		// MOVE IS ALLOWED
		Task(Task<stackDepth>&& other) : task{std::move(other.task) }{
                
            std::copy(std::begin(other.stackBuffer), std::end(other.stackBuffer), std::begin(stackBuffer));
			taskHandle = std::move(other.taskHandle);
			other.taskHandle = 0;
		}

		Task& operator= (Task&& other) {
			task = std::move(other.task);
			std::copy(std::begin(other.stackBuffer), std::end(other.stackBuffer), std::begin(stackBuffer));
			taskHandle = std::move(other.taskHandle);
			other.taskHandle = 0;
			return *this;
		}
		

		// TASK CREATION : https://www.freertos.org/a00019.html
		Task(fct_t _handler, std::string _name, UBaseType_t uxPriority, void* _arg = {}) :
			task{}, stackBuffer{} {
                
                // WAS BUGGY : handler fct was std::function, went out of scope at move. Now impossible with (non-owning) function ptr
			taskHandle = xTaskCreateStatic(_handler, _name.c_str(), stackDepth, _arg, uxPriority, stackBuffer, &task); 
			assert(taskHandle != 0);
		}

		~Task() {
			if(taskHandle != 0) { // do not delete if moved from
				vTaskDelete(taskHandle);
			}
		}

		// TASK CONTROL : https://www.freertos.org/a00112.html
		static void delay(const TickType_t xTicksToDelay) {
			vTaskDelay(xTicksToDelay);
		}

		static xTaskHandle currentHandle(void) {
			return xTaskGetCurrentTaskHandle();
		}

		static void delayUntil(TickType_t* pxPreviousWakeTime, const TickType_t xTimeIncrement) {
			vTaskDelayUntil(pxPreviousWakeTime, xTimeIncrement);
		}

		UBaseType_t priority() {
			return uxTaskPriorityGet(taskHandle);
		}

		void priority(UBaseType_t uxNewPriority) {
			vTaskPrioritySet(taskHandle, uxNewPriority);
		}

		void suspend() {
			vTaskSuspend(taskHandle);
		}

		static void suspendCurrent() {
			vTaskSuspend(NULL);
		}

		void resume() {
			vTaskResume(taskHandle);
		}

		BaseType_t resumeFromISR() {
			return xTaskResumeFromISR(taskHandle);
		}

		BaseType_t abortDelay() {
			return xTaskAbortDelay(taskHandle);
		}

		// TASK UTILITIES : https://www.freertos.org/a00021.html
		// TODO


	};

	static void suspendCurrentTask() {
		vTaskSuspend(NULL);
	}
}
