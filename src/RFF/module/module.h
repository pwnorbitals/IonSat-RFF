#ifndef MODULE_H_INC
#define MODULE_H_INC

#include <tuple>

#include "eventHandler/eventHandler.h"

namespace FFS {
    
    // From https://stackoverflow.com/questions/9831501
    template<typename ...handlers_t>
    class Module {
        protected:
            std::tuple<FFS::EventHandler<handlers_t>...> evtHandlers;
            

        public:             
            Module(std::tuple<FFS::EventHandler<handlers_t>...> _evtHandlers) : evtHandlers{_evtHandlers} {
                
            }
            
            virtual ~Module() {
                
            }
            
            template<typename evt_t>
            void callHandlers(FFS::Event<evt_t> event) {
                /*
                for(auto eh : evtHandlers) {
                    if constexpr (std::is_same<evt_t, eh::event_t>::value) {
                        eh(event);
                    }
                }
                */               
                
                std::apply([event](auto... eh){
                    (..., eh(event));
                }, evtHandlers);
            }

            

    };
}

#endif
