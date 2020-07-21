#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace FFS {
// QUEUES : https://www.freertos.org/a00018.html
    
    template<typename item_t, uint32_t length>
	class Queue {
		QueueHandle_t queueHandle;
		uint8_t storageBuffer[length * sizeof(item_t)];
		StaticQueue_t queue;

	public:
		Queue() {
			queueHandle = xQueueCreateStatic(length, sizeof(item_t), storageBuffer, &queue);
		}

		~Queue() {
			vQueueDelete(queueHandle);
		}

		BaseType_t send( item_t const& pvItemToQueue, TickType_t xTicksToWait) {
			return  xQueueSend(queueHandle, &pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueSendFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}

		BaseType_t sendToBack(item_t const& pvItemToQueue, TickType_t xTicksToWait) {
			return xQueueSendToBack(queueHandle, &pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendToBackFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueSendToBackFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t sendToFront(item_t const& pvItemToQueue, TickType_t xTicksToWait) {
			return xQueueSendToFront(queueHandle, &pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendToFrontFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueSendToFrontFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t receive(item_t& pvBuffer, TickType_t xTicksToWait) {
			return xQueueReceive(queueHandle, &pvBuffer, xTicksToWait);
		}

		BaseType_t receiveFromISR(item_t* pvBuffer, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueReceiveFromISR(queueHandle, pvBuffer, pxHigherPriorityTaskWoken);
		}

		UBaseType_t messagesWaiting() {
			return uxQueueMessagesWaiting(queueHandle);
		}

		UBaseType_t messagesWaitingFromISR() {
			return uxQueueMessagesWaitingFromISR(queueHandle);
		}

		UBaseType_t spacesAvailable() {
			return uxQueueSpacesAvailable(queueHandle);
		}

		BaseType_t reset() {
			return xQueueReset(queueHandle);
		}

		BaseType_t isFullFromISR() {
			return xQueueIsQueueFullFromISR(queueHandle);
		}

		BaseType_t isEmptyFromISR() {
			return xQueueIsQueueEmptyFromISR(queueHandle);
		}

		BaseType_t overwrite(item_t const& pvItemToQueue) {
			return xQueueOverwrite(queueHandle, &pvItemToQueue);
		}

		BaseType_t overwriteFromISR(item_t const& pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueOverwriteFromISR(queueHandle, &pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t peek(item_t* pvBuffer, TickType_t xTicksToWait) {
			return xQueuePeek(queueHandle, pvBuffer, xTicksToWait);
		}

		BaseType_t peekFromISR(item_t* pvBuffer) {
			return xQueuePeekFromISR(queueHandle, pvBuffer);
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
