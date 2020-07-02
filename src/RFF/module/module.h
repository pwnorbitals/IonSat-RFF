#ifndef MODULE_H_INC
#define MODULE_H_INC

#include <tuple>

#include "eventHandler/eventHandler.h"

namespace FFS {
    
    // From https://stackoverflow.com/questions/9831501
    template<typename ...events_t>
    class Module {
        protected:
            std::tuple<FFS::EventHandler<events_t>...> evtHandlers;
            

        public:             
            Module(std::tuple<FFS::EventHandler<events_t>...> _evtHandlers) : evtHandlers{_evtHandlers} {
                
            }
            
            virtual ~Module() {
                
            }
            
            template<typename evt_t>
            void callHandlers(FFS::Event<evt_t> event) const {  
                std::apply([&](auto... eh){
                    (..., eh(event));
                }, evtHandlers);
            }

            

    };
}

#endif
