
#pragma once

#include "eventHandler.h"
#include <atomic>

namespace FFS {

	template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
	class QueuedEventHandler : public EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>  {
        using me_t =      QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>;
		using parent_t =  EventHandler<event_t, me_t>;
        
        enum class msg_e {
            MOVE_ACK
        };
		

	protected:
		Queue<Event<event_t>, maxParallelHandlers> eventsQueue;
        Queue<me_t*, 1> moveQueue;
        Queue<msg_e, 1> acknowledge;
        Mutex protector;
        Task<stackDepth> handlerThread;



	public:
        
        QueuedEventHandler() = delete;
        ~QueuedEventHandler() { 
            protector.take(portMAX_DELAY);
        }
        QueuedEventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        QueuedEventHandler(me_t&& other) : 
            parent_t{other} // PROTECTED BECAUSE COPY CONSTRUCTOR
        { 
            
            moveQueue = {};
            
            other.protector.take(portMAX_DELAY);
            eventsQueue = std::move(other.eventsQueue);
            handlerThread = std::move(other.handlerThread);
            other.protector.give();
            
            other.moveQueue.sendToBack(this);
            msg_e msg;
            acknowledge.receive(msg, portMAX_DELAY);
            assert(msg == msg_e::MOVE_ACK);
            
        }
            
            
        me_t& operator=(me_t&& other) {
            parent_t::operator=(other);
    
            moveQueue = {};
            
            other.protector.take(portMAX_DELAY);
            eventsQueue = std::move(other.eventsQueue);
            handlerThread = std::move(other.handlerThread);
            other.protector.give();
            
            other.moveQueue.sendToBack(this);
            msg_e msg;
            acknowledge.receive(msg, portMAX_DELAY);
            assert(msg == msg_e::MOVE_ACK);
            
            
            
        }
        



		QueuedEventHandler(void (*_handlerFct)(FFS::Event<event_t>*) , std::string _name, UBaseType_t _prio) :
			parent_t{_handlerFct},
            eventsQueue{},
            moveQueue{},
            handlerThread{(void(*)(void*))&me_t::fullHandler, _name, _prio, this}
		{
            protector.take(portMAX_DELAY);
        };
        
        // static, will never go out of scope at a move
        static void fullHandler (me_t* initial_me) { 
            
            me_t* me = initial_me;
			Event<event_t> recvdEvent{};
            me_t* destination;
            auto set = QueueSet<maxParallelHandlers+1>{};
            set.add(me->eventsQueue);
            set.add(me->moveQueue);
            
            me->protector.give();
            
			while(true) {
                
                auto Qhandle = set.select(portMAX_DELAY);
                if(Qhandle == me->eventsQueue.handle()) {
                        me->protector.take(portMAX_DELAY);
                        me->eventsQueue.receive(recvdEvent, 0); 
                        me->handlerFct(&recvdEvent); // not owning the function, so no problem here
                        me->protector.give();
                } else if(Qhandle ==  me->moveQueue.handle()) {
                        me->moveQueue.receive(destination, 0);
                        me = destination;
                        // No queue set add/remove is necessary, because the underlying freeRTOS stay the same during move
                        msg_e ack = msg_e::MOVE_ACK;
                        me->acknowledge.send(ack, 0);
                }
                
                
				


			}
		}
        
        //TODO : move semantics
		bool handleEvent(Event<event_t> const& evt) {
            protector.take(portMAX_DELAY);
			auto res = eventsQueue.sendToBack(evt, 0);
            protector.give();
            return res;
		}
	};
}
