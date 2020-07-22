
#pragma once

#include "eventHandler.h"

namespace FFS {

	template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
	class QueuedEventHandler : public EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>  {
        using me_t =      QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>;
		using parent_t =  EventHandler<event_t, me_t>;
		

	protected:
		std::function<void(void*)> fullHandler; // TODO : use normal member function with std::bind ?
		Queue<Event<event_t>, maxParallelHandlers> eventsQueue;
		Task<stackDepth, void*> handlerThread;




	public:
        
        QueuedEventHandler() = delete;
        QueuedEventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        QueuedEventHandler(me_t&& other) : 
            parent_t{std::move(other)}, 
            fullHandler{std::move(other.fullHandler)}, 
            eventsQueue{std::move(other.eventsQueue)},
            handlerThread{std::move(other.handlerThread)}
            {}
        me_t& operator=(me_t&& other) {
            parent_t::operator=(other);
            fullHandler = std::move(other.fullHandler);
            eventsQueue = std::move(other.eventsQueue);
            handlerThread = std::move(other.handlerThread);
        }
        



		QueuedEventHandler(std::function<void (Event<event_t> const&) > _handlerFct, std::string _name, UBaseType_t _prio) :
			EventHandler<event_t, me_t>{_handlerFct, _name, _prio},
		fullHandler{[this](void* empty) { 

			Event<event_t> recvdEvent{};
			while(true) {
				auto res = eventsQueue.receive(recvdEvent, portMAX_DELAY); // blocks indefinitely

				if(!res) {
					// TODO : count errors ?
					continue;
				}

				parent_t::handlerFct(recvdEvent);


			}
		}},
		eventsQueue{},
		handlerThread{fullHandler, parent_t::name, parent_t::prio}
		{};





		bool handleEvent(Event<event_t> const& evt, TickType_t maxWait = 0) {
			return eventsQueue.sendToBack(evt, maxWait);
		}
	};
}
