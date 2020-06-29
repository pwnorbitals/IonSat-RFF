#ifndef CONTROLLER_H_INC
#define CONTROLLER_H_INC

#include <list>
#include <tuple>
#include <memory>
#include <ctime>
#include <type_traits>

#include "mode/mode.h"
#include "module/module.h"
#include "OS/OS.h"
#include "FFS.h"

namespace FFS {

    


    template<typename ...events_t>
    class Controller {
        protected:
            OSSettings settings;
            std::tuple<FFS::Module<events_t>...> modules;
            std::tuple<FFS::Mode> modes;

        public:
            Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<FFS::Module<events_t>...> _modules) : settings{_settings}, modules{_modules}, modes{_modes} { 
                static_assert(sizeof...(events_t) != 0);
            };
            virtual ~Controller() {};
            
            template<typename chan_t> void emit (chan_t message) const {
                /*
                std::apply([message](auto... chan){
                    (if constexpr(std::is_same_v<chan_t,chan::message_t>) { chan.emit(message) },...);
                }, channels);
                */
                

                std::apply([message](auto... module){(module.callHandlers(message),...);}, modules);
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
