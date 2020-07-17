#pragma once

#include "FFS.h"

#include <functional>
#include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>

namespace FFS {
    


    template<typename event_t, typename Derived>
    class EventHandler {
    protected:

        std::string name;
        UBaseType_t prio;
        std::function<void (Event<event_t>*) > handlerFct;
        

    public:

        EventHandler() = delete;
        EventHandler(EventHandler const& other) = delete;
        EventHandler& operator= (EventHandler const& other) = delete;
        EventHandler(EventHandler&& other) = default;
        EventHandler& operator= (EventHandler&& other) = default;
        ~EventHandler() = default;

        EventHandler(std::function<void (Event<event_t>*) > _handlerFct, std::string _name, UBaseType_t _prio) :
            name{_name}, prio(_prio), handlerFct{_handlerFct}
        { }
        
        Derived& impl(){ return static_cast<Derived&>(*this); }

        template<typename evt_t>
        bool operator()(Event<evt_t> const& evt) {
            if constexpr(std::is_same<evt_t, event_t>::value) {
                return impl().handleEvent(evt);
            }
            
            return false;
        }
    };
    
    template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
    class QueuedEventHandler : public EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>  {
    protected:
        Task<event_t, stackDepth> handlerThread;
        Queue eventsQueue;   
        
        typedef EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>> parent;
        typedef QueuedEventHandler<event_t, stackDepth, maxParallelHandlers> me;
        
    public:
        
        QueuedEventHandler(std::function<void (Event<event_t>*) > _handlerFct, std::string _name, UBaseType_t _prio) : 
            EventHandler<event_t, QueuedEventHandler<event_t, stackDepth, maxParallelHandlers>>{_handlerFct, _name, _prio},
            handlerThread{std::bind(&me::fullHandler, *this, std::placeholders::_1), parent::name, NULL, parent::prio}, eventsQueue{maxParallelHandlers, sizeof(event_t)} 
        {};
        
        
        
        void fullHandler(void* arg) {
            Event<event_t> recvdEvent;
            while(true) {
                eventsQueue.receive(&recvdEvent, portMAX_DELAY); // blocks indefinitely
                parent::handlerFct(&recvdEvent);
            }
        }
        
        bool handleEvent(Event<event_t> const& evt, TickType_t maxWait = 0) {
            return eventsQueue.sendToBack(&evt, maxWait);
        }
    };
    
    template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
    class TaskedEventHandler : public EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>   {
        
    protected:
        // boost::container::static_vector<Task<event_t, stackDepth>, maxParallelHandlers> taskHandlers;
        std::vector<Task<event_t, stackDepth>> taskHandlers; // TODO : switch to static_vector
        Mutex taskHandlersProtector;
        
        typedef EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>> parent;
        typedef TaskedEventHandler<event_t, stackDepth, maxParallelHandlers> me;
        
    public:
          
        
        TaskedEventHandler(std::function<void (Event<event_t>*) > _handlerFct, std::string _name, UBaseType_t _prio) : 
            EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>{_handlerFct, _name, _prio},
            taskHandlers{}, taskHandlersProtector{} 
            {};
        
        void fullHandler(void* arg) {
            auto* event = static_cast<Event<event_t>*>(arg);
            handlerFct(event);

            // Q : problem here ? task may be stopped in the middle of std::remove_if ?
            // A : No problem, FreeRTOS doesn't destroy right away, adds to destroy list and IdleTask destroys later
            taskHandlersProtector.take();
            std::remove_if(
                taskHandlers.begin(), taskHandlers.end(),
                [&event](Task<event_t, stackDepth> const & task) {
                    return task.event == *event;
                }
            );
            taskHandlersProtector.give();
            
            Task<event_t, stackDepth>::suspendCurrent();
        }
        
        bool handleEvent(Event<event_t> const& evt) {
            static uint16_t callCnt = 0;
            
            callCnt++; // Will overflow but is defined behaviour and should not cause problems (low collision probability)
                    // TODO : guarantee no collision is possible
            
            taskHandlersProtector.take();
            taskHandlers.push_back(Task<event_t, stackDepth> {std::bind(&me::fullHandler, this, std::placeholders::_1), parent::name + std::to_string(callCnt), evt, parent::prio});
            // TODO : add bound checking 
            taskHandlersProtector.give();
            
            // TEST ONLY :
            //fullHandler((void*)&evt);
        }
    };
    
    
}
