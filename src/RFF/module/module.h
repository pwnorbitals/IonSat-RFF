#pragma once

#include <tuple>
#include <type_traits>

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
/*
		template<typename type_t>
		struct hasHandler  {
			using value = std::disjunction<
				std::is_same<type_t, typename evtHandlers_t::evt_t>::value...
			>::value;
		};
		*/

        Module() = delete;
        Module(evtHandlers_t& ... modules) : evtHandlers{std::make_tuple(&modules...)}{};
        Module(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        Module(me_t&& other) = default;
        me_t& operator=(me_t&& other) = default;

		bool callHandlers(const void* event, ctti::type_id_t const type) const {
			return std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
				return ([&]{
					using curtype = typename std::remove_reference_t<decltype(*eh)>::evt_t;
					if (type == ctti::type_id<curtype>()) [[unlikely]] { 
						(*eh)(curtype{*((curtype*)event)});
						return true;
					}
					return false;
				}() || ...);
				
			}, evtHandlers);
		}

		bool callHandlersISR(const void* event, ctti::type_id_t const type) const {
			return std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
				return ([&]{
					using curtype = typename std::remove_reference_t<decltype(*eh)>::evt_t;
					if (type == ctti::type_id<curtype>()) [[unlikely]] { 
						(*eh).fromISR(curtype{*((curtype*)event)});
						return true;
					}
					return false;
				}() || ...);
				
			}, evtHandlers);
		}



	};
}
