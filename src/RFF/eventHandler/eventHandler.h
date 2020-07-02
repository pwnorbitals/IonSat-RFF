#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED


#include "event/event.h"
#include <functional>
#include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>


// From : https://stackoverflow.com/questions/7852101
template <class F>
struct lambda_traits : lambda_traits<decltype(&F::operator())>
{ };

template <typename F, typename R, typename... Args>
struct lambda_traits<R(F::*)(Args...)> : lambda_traits<R(F::*)(Args...) const>
{ };

template <class F, class R, class... Args>
struct lambda_traits<R(F::*)(Args...) const> {
    using pointer = typename std::add_pointer<R(Args...)>::type;

    static pointer cify(F&& f) {
        static F fn = std::forward<F>(f);
        return [](Args... args) {
            return fn(std::forward<Args>(args)...);
        };
    }
};

template <class F>
inline typename lambda_traits<F>::pointer cify(F&& f) {
    return lambda_traits<F>::cify(std::forward<F>(f));
}

namespace FFS {
    
    
    

    
    template<typename event_t, uint32_t stackDepth>
    class EventHandler {
        
    
    protected:
        
        std::function<void(FFS::Event<event_t>*)> handlerFct;
        boost::container::static_vector<FFS::Task<event_t, stackDepth>, FFS_MAX_PARALLEL_HANDLERS> taskHandlers;
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
                        [=](FFS::Task<event_t, stackDepth> const& task){ return task.event == evt; }
                    );
                    
                };
                
                
                taskHandlers.push_back(FFS::Task<event_t, stackDepth>{cify([&](void* arg) {
                    
                    // dangerous ! but needed for interoperability with FreeRTOS' void(*)(void*) thread function
                    auto event = reinterpret_cast<FFS::Event<event_t>*>(arg);
                    handlerFct(event);
                    cleanup();
                    
                }), name.c_str(), evt, prio});
                std::cout << "done pushing back" << std::endl;
            }
        }
        
        
    };
}

#endif
