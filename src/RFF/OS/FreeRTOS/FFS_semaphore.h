#pragma once

namespace FFS {
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
                vSemaphoreDelete(semaphoreHandle);
            }

        };
}
