#pragma once

#include "FreeRTOS.h"
#include "RFF.h"

namespace RFF {

    /**
        \brief Group of queues
        
    */
    // QUEUE SETS : https://www.freertos.org/RTOS-queue-sets.html
	template<uint32_t fullLength>
	class QueueSet {
        using me_t = QueueSet<fullLength>;
    protected:
        QueueSetHandle_t QSHandle;
        uint8_t QSStorage[fullLength];
        StaticQueue_t descriptor;
    public:
        QueueSet() {
            // Added :  QueueSetHandle_t xQueueCreateSetStatic( const UBaseType_t uxEventQueueLength, uint8_t *pucQueueSetStorage, StaticQueue_t *pxStaticQueueSet )
            // From : https://www.freertos.org/FreeRTOS_Support_Forum_Archive/June_2018/freertos_static_queue_set_95984609j.html
            QSHandle = xQueueCreateSetStatic(fullLength, QSStorage, &descriptor);
        }
        
        ~QueueSet() {
            
        }
        
        QueueSet(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        
        QueueSet(me_t&& other) : QSHandle{std::move(other.QSHandle)} {
            other.QSHandle = 0;
        }
        
        me_t& operator=(me_t&& other) {
            QSHandle = std::move(other.QSHandle);
            other.QSHandle = 0;
        }
        
        template<typename item_t, uint16_t len>
        bool add(Queue<item_t, len> const& q) {
            return xQueueAddToSet(q.handle(), QSHandle);
        }
        
        bool add(Semaphore const& s) {
            return xQueueAddToSet(s.handle(), QSHandle);
        }
        
        template<typename item_t, uint16_t len>
        bool remove(Queue<item_t, len> const& q) {
            return xQueueRemoveFromSet(q.handle(), QSHandle);
        }
        
        bool remove(Semaphore const& s) {
            return xQueueRemoveFromSet(s.handle(), QSHandle);
        }
        
        QueueSetMemberHandle_t select(TickType_t wait = 0) {
            return xQueueSelectFromSet(QSHandle, wait);
        }
        
        QueueSetMemberHandle_t selectFromISR() {
            return xQueueSelectFromSetFromISR(QSHandle);
        }
    };
    
}
