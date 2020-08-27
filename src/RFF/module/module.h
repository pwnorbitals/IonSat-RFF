#pragma once

#include <tuple>

#include "ctti/type_id.hpp"

#include "eventHandler/eventHandler.h"
#include "RFF.h"

namespace RFF {
    
	/**
		\brief Groups event handlers and dispatches events
	*/
	template <typename ...evtHandlers_t>
	class Module {
        typedef Module<evtHandlers_t...> me_t;
        
	public:
		std::tuple<evtHandlers_t* ...> evtHandlers;


        Module() = delete;
        Module(evtHandlers_t& ... modules) : evtHandlers{std::make_tuple(&modules...)}{};
        Module(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        Module(me_t&& other) = default;
        me_t& operator=(me_t&& other) = default;

		void callHandlers(void* event, ctti::type_id_t type) {
			std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
				([&]{
					using curtype = typename decltype(*eh)::evt_t;
					if constexpr(type == ctti::type_id<curtype>()) { 
						(*eh)(curtype{*event});
					}
				}(), ...);
				
			}, evtHandlers);
		}



	};
}
