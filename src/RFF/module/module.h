#ifndef MODULE_H_INC
#define MODULE_H_INC

#include <tuple>

#include "eventHandler/eventHandler.h"

namespace FFS {
    
    template<typename ...exposed_t>
    class Module {
        protected:
            std::tuple<FFS::Event<exposed_t>, ...> exposedEvts;
            std::tuple<FFS::EventHandler<event_t>, ...> evtHandlers;
            

        public:
            Module(std::tuple<FFS::Event<exposed_t>,...> _exposedEvts, std::tuple<FFS:Task<event_t>,...> _evtHandlers) :
            exposedEvts{_exposedEvts}, evtHandlers{_evtHandlers} {
                
            }
            
            virtual ~Module() {
                
            }
            
            template<typename event_t>
            void callHandlers(FFS::Event<event_t> event) {
                for(auto eh : evtHandlers) {
                    // if event_t == eh::type
                    eh(evt);
                }
            }

            

    };
}

#endif
