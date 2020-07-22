#pragma once

namespace FFS {
	class Semaphore {
	protected:
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
			vSemaphoreDelete(semaphoreHandle);
		}

		UBaseType_t getCount() {
			return uxSemaphoreGetCount(semaphoreHandle);
		}

		bool take(TickType_t ticksToWait = 0) {
			return xSemaphoreTake(semaphoreHandle, ticksToWait);
		}

		bool give() {
			return xSemaphoreGive(semaphoreHandle);
		}

		// TODO
		/*
		bool takeFromISR( TickType_t ticksToWait = 0 ) {
		    return xSemaphoreTakeFromISR( semaphoreHandle, ticksToWait );
		}
		*/



	};

	class Mutex : public Semaphore {
	public:
		Mutex() {
			semaphoreHandle = xSemaphoreCreateMutexStatic(&semaphoreBuffer);
		}
	};
}
