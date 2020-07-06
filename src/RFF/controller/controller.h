#ifndef CONTROLLER_H_INC
#define CONTROLLER_H_INC

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

namespace FFS {

    template <typename T> struct Tag { using type = T; };

    class Controller {
        protected:
            OSSettings settings;
            std::tuple<FFS::Mode> modes;
            std::function<void(std::any)> func;
            
            
        public:
            
            template<typename ...modules, typename eventTags>
            Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<modules...>&& _modules, eventTags event_tags) : settings{_settings}, modes{_modes} { 
                static_assert(sizeof...(modules) != 0);
                
                // From : https://stackoverflow.com/questions/62652638
                func = [this, event_tags, mods{std::move(_modules)}](std::any any_ev) mutable { // invoked _modules' copy constructor which tries to copy the event handlers and the Tasks inside => fail. 
                                                                        // Switch to generalized lambda capture from https://stackoverflow.com/questions/8640393
                    auto f = [&, this, mods2{std::move(mods)}](auto tag) mutable {
                        using EventType = typename decltype(tag)::type;
                        try {
                            auto ev = std::any_cast<EventType>(any_ev);
                            std::apply([&, this] (auto&... module) {((module.callHandlers(FFS::Event<EventType>{ev, this})), ...);}, mods2);
                        } catch(std::bad_any_cast const& e) {};
                        
                    };
                    std::apply([&f](auto&... tags){ (f(tags), ...); }, event_tags);
                };
            }
            
            template<typename evt_t> 
            void emit (evt_t event) {
                func(event);
            }

            void start() {
                std::cout << "Starting controller" << std::endl;
                vTaskStartScheduler(); // TODO : abstract this
                auto start = std::clock();
                while(true) {
                    auto duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

                    if(duration > 30) {
                        std::cout << "End of controller loop" << std::endl;
                        return;
                    }
                }
            }
            
    };


};

#endif
