#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace RFF {
	/**
		\brief First-in First-out (FIFO) data structure for inter-process data exchange
	*/
	// QUEUES : https://www.freertos.org/a00018.html
	template<typename item_t, uint16_t length>
	class Queue {
		QueueHandle_t queueHandle;
		uint8_t storageBuffer[length * sizeof(item_t)];
		StaticQueue_t queue;

	public:
		Queue() {
			queueHandle = xQueueCreateStatic(length, sizeof(item_t), storageBuffer, &queue);
		}

		~Queue() {
            if(queueHandle != 0) {
                vQueueDelete(queueHandle);
            }
		}
		
		// Non-copyable Non-movable because static FreeRTOS queues reference specific memory locations that can't be changed
		// See https://www.freertos.org/xQueueCreateStatic.html
		Queue(Queue<item_t, length> const& other) = delete;
        Queue& operator=(Queue<item_t, length> const& other) = delete;
        Queue(Queue<item_t, length>&& other) = delete;
        Queue& operator=(Queue<item_t, length>&& other) = delete;
        
        QueueHandle_t const& handle() const {
            return queueHandle;
        }

		BaseType_t send(item_t const& pvItemToQueue, TickType_t xTicksToWait = 0) const {
			return  xQueueSend(queueHandle, &pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) const {
			return xQueueSendFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}

		BaseType_t sendToBack(item_t const& pvItemToQueue, TickType_t xTicksToWait = 0) const {
			return xQueueSendToBack(queueHandle, &pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendToBackFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken = nullptr) const {
			return xQueueSendToBackFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t sendToFront(item_t const& pvItemToQueue, TickType_t xTicksToWait = 0) const {
			return xQueueSendToFront(queueHandle, &pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendToFrontFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) const {
			return xQueueSendToFrontFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}

		template<typename receive_type>
		BaseType_t receive(receive_type& pvBuffer, TickType_t xTicksToWait = 0) const {
			static_assert(sizeof(receive_type) == sizeof(item_t), "Received types must be the same size");
			static_assert(std::is_trivially_copyable_v<item_t>, "Received types must be trivially copyable");
			return xQueueReceive(queueHandle, &pvBuffer, xTicksToWait);
		}

		template<typename receive_type>
		BaseType_t receiveFromISR(receive_type& pvBuffer, BaseType_t* pxHigherPriorityTaskWoken) const {
			static_assert(sizeof(receive_type) == sizeof(item_t), "Received types must be the same size");
			static_assert(std::is_trivially_copyable_v<item_t>, "Received types must be trivially copyable");
			return xQueueReceiveFromISR(queueHandle, &pvBuffer, pxHigherPriorityTaskWoken);
		}

		UBaseType_t messagesWaiting() const {
			return uxQueueMessagesWaiting(queueHandle);
		}

		UBaseType_t messagesWaitingFromISR() const {
			return uxQueueMessagesWaitingFromISR(queueHandle);
		}

		UBaseType_t spacesAvailable() const {
			return uxQueueSpacesAvailable(queueHandle);
		}

		BaseType_t reset() const {
			return xQueueReset(queueHandle);
		}

		BaseType_t isFullFromISR() const {
			return xQueueIsQueueFullFromISR(queueHandle);
		}

		BaseType_t isEmptyFromISR() const {
			return xQueueIsQueueEmptyFromISR(queueHandle);
		}

		BaseType_t overwrite(item_t const& pvItemToQueue) const {
			return xQueueOverwrite(queueHandle, &pvItemToQueue);
		}

		BaseType_t overwriteFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) const {
			return xQueueOverwriteFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}

		template<typename receive_type>
		BaseType_t peek(receive_type& pvBuffer, TickType_t xTicksToWait = 0) const {
			static_assert(sizeof(receive_type) == sizeof(item_t), "Received types must be the same size");
			static_assert(std::is_trivially_copyable_v<item_t>, "Received types must be trivially copyable");
			return xQueuePeek(queueHandle, &pvBuffer, xTicksToWait);
		}

		template<typename receive_type>
		BaseType_t peekFromISR(receive_type& pvBuffer) const {
			static_assert(sizeof(receive_type) == sizeof(item_t), "Received types must be the same size");
			static_assert(std::is_trivially_copyable_v<item_t>, "Received types must be trivially copyable");
			return xQueuePeekFromISR(queueHandle, &pvBuffer);
		}


		/*
		void addToRegistry(char *pcQueueName) {
		    vQueueAddToRegistry( queueHandle, pcQueueName);
		}


		void unregister() {
		    vQueueUnregisterQueue( queueHandle );
		}

		const char *getName() {
		    return pcQueueGetName( queueHandle );
		}
		*/




	};
}
