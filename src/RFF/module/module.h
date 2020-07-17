#pragma once

#include <tuple>

#include "eventHandler/eventHandler.h"
#include "event/event.h"
#include "FFS.h"

namespace FFS {
    // From : https://stackoverflow.com/questions/62693894
    template <typename...>
    class Module;

    template <typename ...event_t, typename ...derived>
    class Module<std::tuple<EventHandler<event_t, derived>...>> {
    protected:
        std::tuple<EventHandler<event_t, derived>...> evtHandlers;


    public:
        template<typename ...events_t, typename ...deriveds>
        Module(std::tuple<EventHandler<events_t, deriveds>...>&& _handlers) : evtHandlers{std::move(_handlers) } {}

        template<typename evt_t>
        void callHandlers(Event<evt_t> const& event) {
            std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
                (..., eh(event));
            }, evtHandlers);
        }



    };

    template <typename ... event_t, typename ... derived>
    Module(std::tuple<EventHandler<event_t, derived>...>)
    -> Module<std::tuple<EventHandler<event_t, derived>...>>;

}
