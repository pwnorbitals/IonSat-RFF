#pragma once

#include "RFF.h"

#include <functional>
// #include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>
#include <vector>
#include <algorithm>

#include "unique_function.h"

// From https://stackoverflow.com/questions/38696440

namespace RFF {

    /**
        \brief Handles a series of same-type events
    */
    template<typename event_t, uint16_t prio = 0, uint16_t queueLength = 16, uint32_t stackDepth = 2*configMINIMAL_STACK_SIZE, typename = std::enable_if<std::is_trivially_copyable_v<event_t>>>
	class EventHandler {
    public:
        using me_t =      EventHandler<event_t, prio, queueLength, stackDepth>;
        using handler_t = void(*)(event_t const&);
        using evt_t = event_t;
		
		Queue<event_t, queueLength> eventsQueue;
        Task<prio, stackDepth> handlerThread;
        handler_t handlerFct;
        
        EventHandler() = delete;
        ~EventHandler() = default;
        EventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        EventHandler(me_t&& other) = delete;            
        me_t& operator=(me_t&& other) = delete;
        

		EventHandler(handler_t _handlerFct , std::string _name = "") :
            eventsQueue{},
            handlerThread{(void(*)(void*))&me_t::fullHandler, _name, this},
            handlerFct{_handlerFct}
		{
            
        };
        
        static void fullHandler (me_t* initial_me) { 
            
            me_t* me = initial_me;
            std::aligned_storage_t<sizeof(event_t), alignof(event_t)> recvdEvent;


			while(true) {
                me->eventsQueue.receive(recvdEvent, portMAX_DELAY); 
                me->handlerFct((event_t&)recvdEvent); // not owning the function, so no problem here
                                                      // TODO : Check the cast isn't UB
			}
		}
        
		bool handleEvent(event_t const& evt) {
			auto res = eventsQueue.sendToBack(evt);
            return res;
		}

        bool handleISREvent(event_t const& evt) {
			auto res = eventsQueue.sendToBack(evt);
            return res;
		}

        template<typename evt_t>
		bool fromISR(evt_t&& evt) {
			if constexpr(std::is_same<evt_t, event_t>::value) {
				return handleISREvent(std::move(evt));
			}

			return false;
		}
		
		template<typename evt_t>
		bool operator()(evt_t&& evt) {
			if constexpr(std::is_same<evt_t, event_t>::value) {
				return handleEvent(std::move(evt));
			}

			return false;
		}
	};

}
