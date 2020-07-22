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
        
        using handler_t = std::function<void(Event<event_t> const&)>;

		std::string name;
		UBaseType_t prio;
		handler_t handlerFct;


	public:

		EventHandler() = delete;
		EventHandler(EventHandler const& other) = delete;
		EventHandler& operator= (EventHandler const& other) = delete;
		EventHandler(EventHandler&& other) = default;
		EventHandler& operator= (EventHandler&& other) = default;
		~EventHandler() = default;

		EventHandler(handler_t _handlerFct, std::string _name, UBaseType_t _prio) :
			name{_name}, prio(_prio), handlerFct{_handlerFct}
		{ }

		Derived* impl() { return static_cast<Derived*>(this); }

		template<typename evt_t>
		bool operator()(Event<evt_t> const& evt) {
			if constexpr(std::is_same<evt_t, event_t>::value) {
				return impl()->handleEvent(evt);
			}

			return false;
		}
	};

}

#include "queuedEventHandler.h"
#include "taskedEventHandler.h"
