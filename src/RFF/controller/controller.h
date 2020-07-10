#pragma once

#include <list>
#include <tuple>
#include <memory>
#include <ctime>
#include <type_traits>
#include <any>

#include "mode/mode.h"
#include "module/module.h"
#include "OS/OS.h"
#include "FFS.h"

#include "unique_function.h"

namespace FFS {

    template <typename T> struct Tag {
        using type = T;
    };

    class Controller {
    protected:
        OSSettings settings;
        std::tuple<FFS::Mode> modes;
        unique_function<void (std::any) > func;


    public:

        template<typename ...modules, typename eventTags>
        Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<modules...>&& _modules, eventTags event_tags) : settings{_settings}, modes{_modes} {
    #ifndef FFS_TEST
            static_assert(sizeof...(modules) != 0);
    #endif

            // From : https://stackoverflow.com/questions/62652638
            func = [this, event_tags, mods{std::move(_modules) }](std::any any_ev) mutable {       // invoked _modules' copy constructor which tries to copy the event handlers and the Tasks inside => fail.
                // Switch to generalized lambda capture from https://stackoverflow.com/questions/8640393
                auto f = [this, &any_ev, &mods](auto tag) mutable {
                    using EventType = typename decltype(tag) ::type;
                    try {
                        auto ev = std::any_cast<EventType> (any_ev);
                        std::apply([&, this](auto & ... module) {
                            ((module.callHandlers(FFS::Event<EventType> {ev, this})), ...);
                        }, mods);
                    } catch(std::bad_any_cast const& e) {
                        // assert(!"this should never fail");
                        // <narrator> : In reality, this fails pretty often
                    };

                };
                std::apply([&f](auto & ... tags) {
                    (f(tags), ...);
                }, event_tags);
            };
        }

        template<typename evt_t>
        void emit(evt_t event) {
            func(event);
        }

        void start() {
            std::cout << "Starting controller" << std::endl;
            OSStart();
        }
        
        void stop() {
            std::cout << "Ending controller" << std::endl;
            OSStop();
        }

    };
}
