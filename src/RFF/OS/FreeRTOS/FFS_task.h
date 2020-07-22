#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <boost/container/static_vector.hpp>
#include <algorithm>

namespace FFS {
	template<uint32_t stackDepth, typename arg_t = void*>
	class Task {
        
        using me_t = Task<stackDepth, arg_t>;

	public:

		TaskHandle_t taskHandle;
		StaticTask_t task;
		StackType_t stackBuffer[stackDepth];
		arg_t arg;
		std::function<void(arg_t const&)> handler;
		std::string name;


		// NO COPY
		Task(Task const& other) = delete;
		Task& operator= (Task const& other) = delete;

		// MOVE ALLOWED
		Task(Task<stackDepth, arg_t>&& other) : task{std::move(other.task) },
			stackBuffer{std::move(* (other.stackBuffer)) },
			arg{std::move(other.arg) },
			name{other.name} {
			taskHandle = std::move(other.taskHandle);
			other.taskHandle = 0;
		}

		Task& operator= (Task&& other) {
			task = std::move(other.task);
			std::copy(std::begin(other.stackBuffer), std::end(other.stackBuffer), std::begin(stackBuffer));
			taskHandle = std::move(other.taskHandle);
			name = std::move(other.name);
			other.taskHandle = 0;
			return *this;
		}

		// TASK CREATION : https://www.freertos.org/a00019.html
		Task(std::function<void(arg_t)> _handler, std::string _name, UBaseType_t uxPriority, arg_t _arg = {}) :
			task{}, stackBuffer{}, arg{_arg}, handler{_handler}, name{_name} {
                
                // Trick : function ptr is a non-capturing lambda that takes its context as argument, equivalent to std::function
			taskHandle = xTaskCreateStatic([](void* myself){
                auto me = static_cast<me_t*>(myself);
                me->handler(me->arg);
            }, name.c_str(), stackDepth, this, uxPriority, stackBuffer, &task);
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
