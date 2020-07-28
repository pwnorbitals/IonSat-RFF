
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
        Task<stackDepth> handlerThread;



	public:
        
        QueuedEventHandler() = delete;
        ~QueuedEventHandler() = default;
        QueuedEventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        QueuedEventHandler(me_t&& other) = delete;            
        me_t& operator=(me_t&& other) = delete;
        



		QueuedEventHandler(void (*_handlerFct)(FFS::Event<event_t>*) , std::string _name, UBaseType_t _prio) :
			parent_t{_handlerFct},
            eventsQueue{},
            handlerThread{(void(*)(void*))&me_t::fullHandler, _name, _prio, this}
		{
            
        };
        
        // static, will never go out of scope at a move
        static void fullHandler (me_t* initial_me) { 
            
            me_t* me = initial_me;
			Event<event_t> recvdEvent{};
            
			while(true) {
                
                me->eventsQueue.receive(recvdEvent, portMAX_DELAY); 
                me->handlerFct(&recvdEvent); // not owning the function, so no problem here

                
				


			}
		}
        
        //TODO : move semantics for event handling
		bool handleEvent(Event<event_t> const& evt) {
			auto res = eventsQueue.sendToBack(evt, 0);
            return res;
		}
	};
}
