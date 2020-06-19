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

    template<typename ref_type, typename ...types>
    struct is_any_of: std::integral_constant<bool,(std::is_same_v<ref_type,types>||...)>{};

    template<typename ref_type, typename ...types>
    inline constexpr bool is_any_of_v = is_any_of<ref_type, types...>::value;

    template<typename _message_t>
    class Chan {
        protected:
            std::tuple<std::function<void(_message_t)>> receivers;

        public:
            using message_t = _message_t;

            Chan(const _message_t&, const std::tuple<std::function<void(message_t)>>& rec) : receivers{rec}{}
            
            const Chan& operator<<(const _message_t& m) const {
                this->emit(m);
                return *this;
            }

            template<typename inmsg_t>
            constexpr void emit(const inmsg_t& m) {
                if constexpr (std::is_same_v<message_t, inmsg_t>) {
                    std::apply([m](auto... receiver){(receiver(m),...);},receivers);
                }
            }

    };




    template<typename chan_t>
    Chan<chan_t> make_chan(std::function<void(chan_t)> receivers) {
        return Chan{chan_t{}, std::make_tuple(receivers)};
    }


    template<typename ...chans_t>
    class Controller {
        protected:
            OSSettings settings;
            std::tuple<Chan<chans_t>...> channels;
            std::tuple<std::unique_ptr<FFS::Module>> modules;
            std::tuple<FFS::Mode> modes;

        public:
            Controller(OSSettings _settings, std::tuple<FFS::Mode> _modes, std::tuple<Chan<chans_t>...> _channels) : settings{_settings}, channels{_channels}, modules{}, modes{_modes} { };
            virtual ~Controller() {};
            
            template<typename chan_t> void emit (chan_t message) const {
                /*
                std::apply([message](auto... chan){
                    (if constexpr(std::is_same_v<chan_t,chan::message_t>) { chan.emit(message) },...);
                }, channels);
                */

                std::apply([message](auto... chan){(chan.emit(message),...);}, channels);
            };

            void start() {
                std::cout << "Starting controller" << std::endl;
                while(true) {
                    auto start = std::clock();
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
