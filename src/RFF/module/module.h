#pragma once

#include <tuple>

#include "eventHandler/eventHandler.h"
#include "event/event.h"
#include "FFS.h"

namespace FFS {

    template <typename ...evtHandlers_t>
    class Module {
    protected:
        std::tuple<evtHandlers_t...> evtHandlers;


    public:
        template<typename ...events_t, typename ...deriveds>
        Module(std::tuple<evtHandlers_t...>&& _handlers) : evtHandlers{std::move(_handlers) } {}

        template<typename evt_t>
        void callHandlers(evt_t const& event) {
            std::apply([&](auto & ... eh) {    // lvalue reference argument because move would consume the event handlers
                (..., eh(event));
            }, evtHandlers);
        }



    };
}
