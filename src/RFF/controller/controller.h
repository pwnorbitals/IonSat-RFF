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
            Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<modules...> _modules, eventTags event_tags) : settings{_settings}, modes{_modes} { 
                static_assert(sizeof...(modules) != 0);
                
                func = [=](std::any any_ev){
                    auto f = [&](auto tag){
                        using EventType = typename decltype(tag)::type;
                        if (auto* ev = std::any_cast<EventType>(&any_ev)) {
                            std::apply([=](auto&&... module) {((module.callHandlers(FFS::Event<EventType>{*ev, this})), ...);}, _modules);
                        }
                    };
                    std::apply([&f](auto&&... tags){ (f(tags), ...); }, event_tags);
                };
            };
            virtual ~Controller() {};
            
            template<typename evt_t> 
            void emit (evt_t event) {
                func(event);
            };

            void start() {
                std::cout << "Starting controller" << std::endl;
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


}

#endif
