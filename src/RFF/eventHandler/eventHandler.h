#pragma once

#include "event/event.h"
#include "FFS.h"

#include <functional>
#include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>

namespace FFS {


    template<typename event_t, uint32_t stackDepth>
    class EventHandler {


    protected:

        
        // boost::container::static_vector<Task<event_t, stackDepth>, FFS_MAX_PARALLEL_HANDLERS> taskHandlers;
        std::vector<Task<event_t, stackDepth>> taskHandlers; // TODO : switch to static_vector
        std::string name;
        UBaseType_t prio;
        std::function<void (void*)> fullHandler;
        std::function<void (Event<event_t>*) > handlerFct;






    public:

        EventHandler() = delete;
        EventHandler(EventHandler const& other) = delete;
        EventHandler& operator= (EventHandler const& other) = delete;
        EventHandler(EventHandler&& other) = default;
        EventHandler& operator= (EventHandler&& other) = default;
        ~EventHandler() = default;

        EventHandler(std::function<void (Event<event_t>*) > _handlerFct, std::string _name, UBaseType_t _prio) :
            taskHandlers{},name{_name}, prio(_prio),  handlerFct{_handlerFct}{
                
                std::cout << "creating" << std::endl;
                
                fullHandler = [this](void* arg) {
                    // dangerous ! but needed for interoperability with FreeRTOS' void(*)(void*) thread function
                    std::cout << "casting fct pointer" << std::endl;
                    auto* event = reinterpret_cast<Event<event_t>*>(arg);

                    std::cout << "handling event" << std::endl;
                    handlerFct(event);


                    std::cout << "cleaning up" << std::endl;
                    std::remove_if(
                        taskHandlers.begin(), taskHandlers.end(),
                        [&event](Task<event_t, stackDepth> const & task) {
                            return task.event == *event;
                        }
                    );
                };

        }

        template<typename evt_t>
        void operator()(Event<evt_t> const& evt) {
            if constexpr(std::is_same<evt_t, event_t>::value) {
                std::cout << "pushing back" << std::endl;
                taskHandlers.push_back(Task<event_t, stackDepth> {fullHandler, name.c_str(), evt, prio});
                std::cout << "done pushing back" << std::endl;
            }
        }


    };
}
