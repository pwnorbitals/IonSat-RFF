#ifndef MODULE_H_INC
#define MODULE_H_INC

#include <tuple>

#include "eventHandler/eventHandler.h"

namespace FFS {

// From : https://stackoverflow.com/questions/62693894
	template <typename...>
	class Module;

	template <typename ... event_t, uint32_t ... stackDepth>
	class Module<std::tuple<EventHandler<event_t, stackDepth>...>> {
	protected:
		std::tuple<EventHandler<event_t, stackDepth>...> evtHandlers;


	public:
		template<typename ...events_t, uint32_t ...stackDepths>
		Module(std::tuple<EventHandler<events_t, stackDepths>...>&& _handlers) : evtHandlers{std::move(_handlers) } {}

		template<typename evt_t>
		void callHandlers(Event<evt_t> const& event) {
			std::apply([&](auto & ... eh) {    // lvalue reference argument because rvalue reference would consume the event handlers
				(..., eh(event));
			}, evtHandlers);
		}



	};

	template <typename ... event_t, std::uint32_t ... stackDepth>
	Module(std::tuple<EventHandler<event_t, stackDepth>...>)
	-> Module<std::tuple<EventHandler<event_t, stackDepth>...>>;
}

#endif
