#pragma once

#include "FFS.h"

#include <functional>
#include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>
#include <vector>
#include <algorithm>

#include "lambda.h"
#include "unique_function.h"

namespace FFS {



	template<typename event_t, typename Derived>
	class EventHandler {
	protected:
        
        using handler_t = void(*)(Event<event_t>*);
		handler_t handlerFct;


	public:

		EventHandler() = delete;
		EventHandler(EventHandler const& other) = delete;
		EventHandler& operator= (EventHandler const& other) = delete;
		EventHandler(EventHandler&& other): handlerFct{std::move(other.handlerFct)}{
            /*
            objLock = {};
            other.objLock.take();
            
            name = std::move(other.name);
            prio = std::move(other.prio);
            handlerFct = std::move(other.handlerFct);
            
            other.objLock.give();
            */
        }
		EventHandler& operator= (EventHandler&& other) {
           
            handlerFct = std::move(other.handlerFct);
        };
		~EventHandler(){
            
        };

		EventHandler(handler_t _handlerFct) :
			handlerFct{_handlerFct}
		{ }

		Derived* impl() { return static_cast<Derived*>(this); }

		template<typename evt_t>
		bool operator()(Event<evt_t>&& evt) {
			if constexpr(std::is_same<evt_t, event_t>::value) {
				return impl()->handleEvent(std::move(evt));
			}

			return false;
		}
	};

}

#include "queuedEventHandler.h"
#include "taskedEventHandler.h"
