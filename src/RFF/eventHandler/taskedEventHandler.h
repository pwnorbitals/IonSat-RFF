
#pragma once

#include "eventHandler.h"
#include <list>

namespace FFS {
	template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
	class TaskedEventHandler : public EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>   {

        using me_t = TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>;
		using parent_t = EventHandler<event_t, me_t>;
        using task_t = Task<stackDepth>;

	protected:
		// boost::container::static_vector<Task<event_t, stackDepth>, maxParallelHandlers> taskHandlers;
		std::list<task_t> taskHandlers; // TODO : switch to static_vector
		Mutex taskHandlersProtector;
        std::string name;
        UBaseType_t prio;
        Queue<Event<event_t>, maxParallelHandlers> waitingEvents;



	public:

        TaskedEventHandler() = delete;
        TaskedEventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        TaskedEventHandler(me_t&& other) : 
            parent_t{std::move(other)},
            taskHandlers{std::move(other.taskHandlers)},
            taskHandlersProtector{std::move(other.taskHandlersProtector)}
            {};
        me_t& operator=(me_t&& other) {
            parent_t::operator=(std::move(other));
            taskHandlers = std::move(other.taskHandlers);
            taskHandlersProtector = std::move(other.taskHandlersProtector);
        }

		TaskedEventHandler(void(*_handlerFct)(Event<event_t>*), std::string _name, UBaseType_t _prio) :
			parent_t{_handlerFct},
		taskHandlers{}, taskHandlersProtector{}, name{_name}, prio{_prio}
		{};


		bool handleEvent(Event<event_t>&& evt) {
			static uint16_t callCnt = 0;

			callCnt++; // Will overflow but is defined behaviour and should not cause problems (low collision probability)
                       // Also, name collision is no problem from the OS point of view (it's only for debugging)
                       
            waitingEvents.sendToBack(evt);

			taskHandlersProtector.take();
			taskHandlers.emplace_back((void(*)(void*))&me_t::fullHandler, name + std::to_string(callCnt), prio, this);
			// TODO : add bound checking
			taskHandlersProtector.give();

			// TEST ONLY :
			//fullHandler((void*)&evt);

			return true;
		}
		
		static void fullHandler(me_t* me) {
            
            Event<event_t> recvdEvent;
            if(!me->waitingEvents.receive(recvdEvent, 0)) {
                return;
            }
			
            me->handlerFct(&recvdEvent);
            
            
            
			auto curHandle = task_t::currentHandle();

			// Q : problem here ? task may be halted in the middle of the operation ?
			// A : No problem, FreeRTOS doesn't destroy right away, adds to destroy list and IdleTask destroys later
			me->taskHandlersProtector.take();
			// me->taskHandlers.erase(toErase, me->taskHandlers.end());
            me->taskHandlers.remove_if([&curHandle](task_t const& task) {
				return task.taskHandle == curHandle;
			});
			me->taskHandlersProtector.give();

			FFS::suspendCurrentTask();
		}
	};
}
