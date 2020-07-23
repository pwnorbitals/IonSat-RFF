
#pragma once

#include "eventHandler.h"
#include <atomic>

namespace FFS {

	template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
	class QueuedEventHandler : public EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>  {
        using me_t =      QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>;
		using parent_t =  EventHandler<event_t, me_t>;
		

	protected:
		Queue<Event<event_t>, maxParallelHandlers> eventsQueue;
		Task<stackDepth> handlerThread;
        Queue<me_t*, 1> moveQueue;



	public:
        
        QueuedEventHandler() = delete;
        ~QueuedEventHandler() = default;
        QueuedEventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        QueuedEventHandler(me_t&& other) : 
            parent_t{std::move(other)}, 
            eventsQueue{std::move(other.eventsQueue)},
            handlerThread{std::move(other.handlerThread)}{ }
            
            
        me_t& operator=(me_t&& other) {
            parent_t::operator=(other);
            eventsQueue = std::move(other.eventsQueue);
            handlerThread = std::move(other.handlerThread);
            
            moveQueue = {};
            other.moveQueue.sendToBack(this);
        }
        



		QueuedEventHandler(void (*_handlerFct)(FFS::Event<event_t>*) , std::string _name, UBaseType_t _prio) :
			parent_t{_handlerFct},
            eventsQueue{},
            handlerThread{(void(*)(void*))&me_t::fullHandler, _name, _prio, this},
            moveQueue{}
		{};
        
        // static, will never go out of scope at a move
        static void fullHandler (me_t** initial_me) { 
            // BUG : IMPLICITELY CAPTURES THIS WHEN MOVE HAPPENS, USES DELETED THIS
            // SOLUTION : ADAPT THIS
            
			Event<event_t> recvdEvent{};
            me_t* me = *initial_me;
            me_t* destination;
            
            auto set = QueueSet<maxParallelHandlers+1>{};
            set.add(me->eventsQueue);
            set.add(me->moveQueue);
            
			while(true) {
                
                auto Qhandle = set.select(portMAX_DELAY);
                if(Qhandle == me->eventsQueue.handle()) {
                        me->eventsQueue.receive(recvdEvent, 0); 
                        me->handlerFct(&recvdEvent); // not owning the function, so no problem here
                } else if(Qhandle ==  me->moveQueue.handle()) {
                        me->moveQueue.receive(destination, 0);
                        set.remove(me->eventsQueue);
                        set.remove(me->moveQueue);
                        
                        me = destination;
                        
                        set.add(me->eventsQueue);
                        set.add(me->moveQueue);
                }
                
                
				


			}
		}
        
        //TODO : move semantics
		bool handleEvent(Event<event_t> const& evt, TickType_t maxWait = 0) {
			return eventsQueue.sendToBack(evt, maxWait);
		}
	};
}
