#ifndef FREERTOS_H_INC
#define FREERTOS_H_INC

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

namespace FFS {
    
    struct OSSettings {
        
    };

    class Task {

        TaskHandle_t taskHandle;
        StaticTask_t task;
        StackType_t StackBuffer;

    public:

            // TASK CREATION : https://www.freertos.org/a00019.html
        Task(TaskFunction_t pxTaskCode, const char * const pcName, const uint32_t ulStackDepth, void * const pvParameters, UBaseType_t uxPriority) {
            taskHandle = xTaskCreateStatic(pxTaskCode, pcName, ulStackDepth, pvParameters, uxPriority, &StackBuffer, &task);
        }

        ~Task() {
            vTaskDelete( taskHandle );
        }

            // TASK CONTROL : https://www.freertos.org/a00112.html
        static void delay(const TickType_t xTicksToDelay) {
            vTaskDelay( xTicksToDelay );
        }

        static void delayUntil(TickType_t *pxPreviousWakeTime, const TickType_t xTimeIncrement) {
            vTaskDelayUntil( pxPreviousWakeTime, xTimeIncrement );
        }

        UBaseType_t priority( ) {
            return uxTaskPriorityGet( taskHandle );
        }

        void priority( UBaseType_t uxNewPriority) {
            vTaskPrioritySet( taskHandle, uxNewPriority );
        }

        void suspend() {
            vTaskSuspend( taskHandle );
        }

        void resume() {
            vTaskResume( taskHandle );
        }

        BaseType_t resumeFromISR() {
            return xTaskResumeFromISR( taskHandle );
        }

        BaseType_t abortDelay() {
            return xTaskAbortDelay( taskHandle );
        }

            // TASK UTILITIES : https://www.freertos.org/a00021.html
        // TODO


    };


        // QUEUES : https://www.freertos.org/a00018.html
    class Queue {
        QueueHandle_t queueHandle;
        uint8_t storageBuffer;
        StaticQueue_t queue;

    public:
        Queue(UBaseType_t uxQueueLength,
              UBaseType_t uxItemSize,
              uint8_t *pucQueueStorageBuffer,
              StaticQueue_t *pxQueueBuffer) {

            queueHandle = xQueueCreateStatic(
                                         uxQueueLength,
                                         uxItemSize,
                                         &storageBuffer,
                                         &queue );

        }

        ~Queue() {
            vQueueDelete( queueHandle );
        }

        BaseType_t send(const void * pvItemToQueue, TickType_t xTicksToWait) {
            return  xQueueSend(queueHandle, pvItemToQueue, xTicksToWait);
        }

        BaseType_t sendFromISR(const void *pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueSendFromISR(queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }

        BaseType_t sendToBack( const void * pvItemToQueue, TickType_t xTicksToWait) {
            return xQueueSendToBack( queueHandle, pvItemToQueue, xTicksToWait);
        }

        BaseType_t sendToBackFromISR(const void *pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken){
            return xQueueSendToBackFromISR (queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }


        BaseType_t sendToFront(const void * pvItemToQueue, TickType_t xTicksToWait ) {
            return xQueueSendToFront( queueHandle, pvItemToQueue, xTicksToWait);
        }

        BaseType_t sendToFrontFromISR(const void *pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueSendToFrontFromISR ( queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }


        BaseType_t receive(void *pvBuffer, TickType_t xTicksToWait) {
            return xQueueReceive(queueHandle, pvBuffer, xTicksToWait);
        }

        BaseType_t receiveFromISR(void *pvBuffer, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueReceiveFromISR(queueHandle, pvBuffer, pxHigherPriorityTaskWoken);
        }

        UBaseType_t messagesWaiting() {
            return uxQueueMessagesWaiting( queueHandle );
        }

        UBaseType_t messagesWaitingFromISR() {
            return uxQueueMessagesWaitingFromISR( queueHandle );
        }

        UBaseType_t spacesAvailable() {
            return uxQueueSpacesAvailable( queueHandle );
        }

        BaseType_t reset() {
            return xQueueReset( queueHandle );
        }

        BaseType_t isFullFromISR() {
            return xQueueIsQueueFullFromISR( queueHandle );
        }

        BaseType_t isEmptyFromISR() {
            return xQueueIsQueueEmptyFromISR( queueHandle );
        }

        BaseType_t overwrite(const void * pvItemToQueue) {
            return xQueueOverwrite( queueHandle, pvItemToQueue );
        }

        BaseType_t overwriteFromISR(const void * pvItemToQueue, BaseType_t *pxHigherPriorityTaskWoken) {
            return xQueueOverwriteFromISR( queueHandle, pvItemToQueue, pxHigherPriorityTaskWoken);
        }


        BaseType_t peek(void *pvBuffer, TickType_t xTicksToWait) {
            return xQueuePeek( queueHandle, pvBuffer, xTicksToWait );
        }

        BaseType_t peekFromISR(void *pvBuffer) {
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

    class Semaphore {
        SemaphoreHandle_t semaphoreHandle;
        StaticSemaphore_t semaphoreBuffer;

    public:
        Semaphore() {
            semaphoreHandle = xSemaphoreCreateBinaryStatic(&semaphoreBuffer);
        }

        Semaphore(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount) {
            semaphoreHandle = xSemaphoreCreateCountingStatic(uxMaxCount, uxInitialCount, &semaphoreBuffer);
        }

        ~Semaphore() {
            vSemaphoreDelete( semaphoreHandle );
        }

    };



}

extern "C" {
    void vMainQueueSendPassed( void )
    {
        /* This is just an example implementation of the "queue send" trace hook. */

    }

    void vApplicationIdleHook( void )
    {

        #ifdef __GCC_POSIX__
            struct timespec xTimeToSleep, xTimeSlept;
                /* Makes the process more agreeable when using the Posix simulator. */
                xTimeToSleep.tv_sec = 1;
                xTimeToSleep.tv_nsec = 0;
                nanosleep( &xTimeToSleep, &xTimeSlept );
        #endif
    }

    void vApplicationGetIdleTaskMemory( void ) {

    }
    void vApplicationGetTimerTaskMemory( void ) {

    }

    void* pvTaskGetThreadLocalStoragePointer( void ){

    }

    void vTaskSetThreadLocalStoragePointer( void ){

    }

    TaskHandle_t xTaskGetIdleTaskHandle() {

    }

}


#endif
