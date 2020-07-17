#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace FFS {
// QUEUES : https://www.freertos.org/a00018.html
	class Queue {
		QueueHandle_t queueHandle;
		uint8_t storageBuffer;
		StaticQueue_t queue;

	public:
		Queue(UBaseType_t uxQueueLength,
		      UBaseType_t uxItemSize) {

			queueHandle = xQueueCreateStatic(
			                  uxQueueLength,
			                  uxItemSize,
			                  &storageBuffer,
			                  &queue);

		}

		~Queue() {
			vQueueDelete(queueHandle);
		}

		BaseType_t send(const void* pvItemToQueue, TickType_t xTicksToWait) {
			return  xQueueSend(queueHandle, pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendFromISR(const void* pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueSendFromISR(queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
		}

		BaseType_t sendToBack(const void* pvItemToQueue, TickType_t xTicksToWait) {
			return xQueueSendToBack(queueHandle, pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendToBackFromISR(const void* pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueSendToBackFromISR(queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t sendToFront(const void* pvItemToQueue, TickType_t xTicksToWait) {
			return xQueueSendToFront(queueHandle, pvItemToQueue, xTicksToWait);
		}

		BaseType_t sendToFrontFromISR(const void* pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueSendToFrontFromISR(queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t receive(void* pvBuffer, TickType_t xTicksToWait) {
			return xQueueReceive(queueHandle, pvBuffer, xTicksToWait);
		}

		BaseType_t receiveFromISR(void* pvBuffer, BaseType_t* pxHigherPriorityTaskWoken) {
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

		BaseType_t overwrite(const void* pvItemToQueue) {
			return xQueueOverwrite(queueHandle, pvItemToQueue);
		}

		BaseType_t overwriteFromISR(const void* pvItemToQueue, BaseType_t* pxHigherPriorityTaskWoken) {
			return xQueueOverwriteFromISR(queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
		}


		BaseType_t peek(void* pvBuffer, TickType_t xTicksToWait) {
			return xQueuePeek(queueHandle, pvBuffer, xTicksToWait);
		}

		BaseType_t peekFromISR(void* pvBuffer) {
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
