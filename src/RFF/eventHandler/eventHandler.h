#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED

#include "event/event.h"
#include "short_alloc.h"
#include <functional>

// From : https://howardhinnant.github.io/stack_alloc.html
template <class T, std::size_t BufSize = FFS_MAX_PARALLEL_HANDLERS>
using SmallVector = std::vector<T, short_alloc<T, BufSize, alignof(T)>>;

namespace FFS {
    
    template<typename _evt_t>
    class EventHandler {
        
        
        
    public:
        using event_t = _evt_t;
    
    protected:
        std::function<void(std::unique_ptr<FFS::Event<event_t>>)> handlerFct;
        // SmallVector<FFS::Task>::allocator_type::arena_type arena;
        std::vector<FFS::Task/*, FFS_MAX_PARALLEL_HANDLERS*/> taskHandlers;
        std::string name;
        uint32_t stackDepth;
    
        
        
        
        
    
    public:
        
        EventHandler(std::function<void(std::unique_ptr<FFS::Event<event_t>>)> _handlerFct, std::string _name, uint32_t const _stackDepth) :                        
            handlerFct{_handlerFct},/* arena{},*/ taskHandlers{/*arena*/}, name{_name}, stackDepth{_stackDepth} {
            
        }
        
        template<typename evt_t>
        void operator()(FFS::Event<evt_t> evt) {
            auto prio = 1;
            if constexpr (std::is_same<evt_t, event_t>::value) {
                taskHandlers.push_back(FFS::Task{handlerFct, name.c_str(), stackDepth, std::make_unique(evt), prio});
            }
        }
        
        
    };
}




#endif
