
#pragma once

#include "eventHandler.h"

namespace FFS {

    template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
    class QueuedEventHandler : public EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>  {
        typedef EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>  parent_t;
        typedef QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>                         me_t;
        
    protected:
        void(*fullHandler)(void*); // TODO : use normal member function with std::bind ?
        Queue<Event<event_t>, maxParallelHandlers> eventsQueue; 
        Task<me_t*, stackDepth> handlerThread;
          
        
        
        
    public:
        
        
        
        QueuedEventHandler(std::function<void (Event<event_t> const&) > _handlerFct, std::string _name, UBaseType_t _prio) : 
            EventHandler<event_t, me_t>{_handlerFct, _name, _prio},
            fullHandler{[]( void* myself ) { // myself is a pointer to the this pointer !
                auto** that = static_cast<me_t**>(myself);
                auto* me = *that;
                Event<event_t> recvdEvent;
                while(true) {
                    auto res = me->eventsQueue.receive(recvdEvent, portMAX_DELAY); // blocks indefinitely
                    
                    if(!res) {
                        // TODO : count errors ?
                        continue;
                    }
                    
                    me->handlerFct(recvdEvent);
                    
                    
                }
            }},
            eventsQueue{},
            handlerThread{fullHandler, parent_t::name, parent_t::prio, this}
        {};
        
        
        
        
        
        bool handleEvent(Event<event_t> const& evt, TickType_t maxWait = 0) {
            return eventsQueue.sendToBack(evt, maxWait);
        }
    };
}
