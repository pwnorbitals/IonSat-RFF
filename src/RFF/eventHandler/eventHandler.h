#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED


#include "event/event.h"
#include <functional>
#include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>

// From : https://stackoverflow.com/questions/7852101
struct Lambda {
    template<typename Tret, typename T>
    static Tret lambda_ptr_exec(void* data) {
        return (Tret) (*(T*)fn<T>())(data);
    }

    template<typename Tret = void, typename Tfp = Tret(*)(void*), typename T>
    static Tfp ptr(T&& t) {
        fn<T>(&t);
        return (Tfp) lambda_ptr_exec<Tret, T>;
    }

    template<typename T>
    static void* fn(void* new_fn = nullptr) {
        static void* fn;
        if (new_fn != nullptr)
            fn = new_fn;
        return fn;
    }
};

namespace FFS {
    template<typename event_t, uint32_t stackDepth>
    class EventHandler {
        
    
    protected:
        
        std::function<void(FFS::Event<event_t>*)> handlerFct;
        // boost::container::static_vector<FFS::Task<event_t, stackDepth>, FFS_MAX_PARALLEL_HANDLERS> taskHandlers;
        std::vector<FFS::Task<event_t, stackDepth>> taskHandlers;
        std::string name;
    
        
        
        
        
    
    public:
        
        EventHandler( std::function<void(FFS::Event<event_t>*)> _handlerFct, std::string _name) :                        
            handlerFct{_handlerFct}, taskHandlers{}, name{_name} {
            
        }
        
        template<typename evt_t>
        void operator()(FFS::Event<evt_t> const& evt) {
            if constexpr (std::is_same<evt_t, event_t>::value) {
                auto prio = UBaseType_t{1};
                std::cout << "pushing back" << std::endl;
                
                auto cleanup = [=]() { 
                    std::remove_if(
                        taskHandlers.begin(), taskHandlers.end(), 
                        [&](FFS::Task<event_t, stackDepth> const& task){ return task.event == evt; }
                    );
                    
                };
                
                void (*hf)(void*) = Lambda::ptr([&](void* arg) {
                    // dangerous ! but needed for interoperability with FreeRTOS' void(*)(void*) thread function
                    auto event = reinterpret_cast<FFS::Event<event_t>*>(arg);
                    handlerFct(event);
                    cleanup();
                });
                
                taskHandlers.push_back(FFS::Task<event_t, stackDepth>{hf, name.c_str(), evt, prio});
                std::cout << "done pushing back" << std::endl;
            }
        }
        
        
    };
}

#endif
