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
    
    
    


    
    template<typename _evt_t>
    class EventHandler {
        
        
        
    public:
        using event_t = _evt_t;
    
    protected:
        
        std::function<void(FFS::Event<event_t>*)> handlerFct;
        boost::container::static_vector<FFS::Task<event_t>, FFS_MAX_PARALLEL_HANDLERS> taskHandlers;
        std::string name;
        uint32_t stackDepth;
    
        
        
        
        
    
    public:
        
        EventHandler( std::function<void(FFS::Event<event_t>*)> _handlerFct, std::string _name, uint32_t const _stackDepth) :                        
            handlerFct{_handlerFct}, taskHandlers{}, name{_name}, stackDepth{_stackDepth} {
            
        }
        
        template<typename evt_t>
        void operator()(FFS::Event<evt_t> evt) {
            auto prio = 1;
            if constexpr (std::is_same<evt_t, event_t>::value) {
                std::cout << "pushing back" << std::endl;
                
                auto cursize = taskHandlers.size();
                auto cleanup = [&]() { taskHandlers.erase(taskHandlers.begin() + cursize); };
                
                taskHandlers.push_back(FFS::Task{cify([&](void* arg) {
                    
                    // dangerous ! but needed for interoperability with FreeRTOS' void(*)(void*) thread function
                    auto event = reinterpret_cast<FFS::Event<event_t>*>(arg);
                    handlerFct(event);
                    cleanup();
                    
                }), name.c_str(), stackDepth, evt, prio});
                std::cout << "done pushing back" << std::endl;
            }
        }
        
        
    };
}

#endif
