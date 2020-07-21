
#pragma once

#include "eventHandler.h"

namespace FFS {
    template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
    class TaskedEventHandler : public EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>   {
        
    protected:
        void(*fullHandler)(void*);
        // boost::container::static_vector<Task<event_t, stackDepth>, maxParallelHandlers> taskHandlers;
        std::vector<Task<TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>*, stackDepth>> taskHandlers; // TODO : switch to static_vector
        Mutex taskHandlersProtector;
        Queue<Event<event_t>, maxParallelHandlers> waitingEvents;
        
        typedef EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>> parent_t;
        typedef TaskedEventHandler<event_t, stackDepth, maxParallelHandlers> me_t;
        
    public:
        
        
        TaskedEventHandler(std::function<void (Event<event_t> const&) > _handlerFct, std::string _name, UBaseType_t _prio) : 
            EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>{_handlerFct, _name, _prio},
            fullHandler{[](void* myself) {
                
                auto* me = static_cast<TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>*>(myself);
                
                
                Event<event_t> event;
                auto res = me->waitingEvents.receive(event, 0);
                if(res != pdFALSE) { 
                    me->handlerFct( event );
                }

                
                auto curHandle{Task<event_t, stackDepth>::currentHandle()};
                auto toErase = std::remove_if(
                    me->taskHandlers.begin(), me->taskHandlers.end(),
                    [curHandle](Task<TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>*, stackDepth> const & task) {
                        return task.taskHandle == curHandle;
                    }
                );
                
                // Q : problem here ? task may be halted in the middle of the operation ?
                // A : No problem, FreeRTOS doesn't destroy right away, adds to destroy list and IdleTask destroys later
                me->taskHandlersProtector.take();
                me->taskHandlers.erase(toErase, me->taskHandlers.end());
                me->taskHandlersProtector.give();
                
                Task<event_t, stackDepth>::suspendCurrent();
            }}, 
            taskHandlers{}, taskHandlersProtector{}, waitingEvents{}
        {};
        
        
        bool handleEvent(Event<event_t> const& evt) {
            static uint16_t callCnt = 0;
            
            callCnt++; // Will overflow but is defined behaviour and should not cause problems (low collision probability)
                       // Also, name collision is no problem from the OS point of view (it's only for debugging)
                       
            auto res = waitingEvents.sendToBack(evt, 0);
            if(res == pdFALSE) { return false; }
            
            taskHandlersProtector.take();
            taskHandlers.push_back(Task<TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>*, stackDepth> {fullHandler, parent_t::name + std::to_string(callCnt), parent_t::prio, this});
            // TODO : add bound checking 
            taskHandlersProtector.give();
            
            // TEST ONLY :
            //fullHandler((void*)&evt);
            
            return true;
        }
    };
}
