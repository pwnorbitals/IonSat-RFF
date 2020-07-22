#pragma once

#include <tuple>

#include "eventHandler/eventHandler.h"
#include "event/event.h"
#include "FFS.h"

namespace FFS {

	template <typename ...evtHandlers_t>
	class Module {
        typedef Module<evtHandlers_t...> me_t;
        
	protected:
		std::tuple<evtHandlers_t...> evtHandlers;


	public:
        
        Module() = delete;
        Module(std::tuple<evtHandlers_t...>&& modules) : evtHandlers{std::move(modules)}{};
        Module(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        Module(me_t&& other) = default;
        me_t& operator=(me_t&& other) = default;

		template<typename evt_t>
		void callHandlers(evt_t const& event) {
			std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
				(..., eh(evt_t{event}));
			}, evtHandlers);
		}



	};
}
