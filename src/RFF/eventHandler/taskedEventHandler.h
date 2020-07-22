
#pragma once

#include "eventHandler.h"

namespace FFS {
	template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
	class TaskedEventHandler : public EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>   {

        using me_t = TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>;
		using parent_t = EventHandler<event_t, me_t>;
        using task_t = Task<stackDepth, Event<event_t>&&>;

	protected:
		// boost::container::static_vector<Task<event_t, stackDepth>, maxParallelHandlers> taskHandlers;
		std::vector<task_t> taskHandlers; // TODO : switch to static_vector
		Mutex taskHandlersProtector;



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

		TaskedEventHandler(std::function<void(Event<event_t> const&)> _handlerFct, std::string _name, UBaseType_t _prio) :
			parent_t{_handlerFct, _name, _prio},
		taskHandlers{}, taskHandlersProtector{}
		{};


		bool handleEvent(Event<event_t>&& evt) {
			static uint16_t callCnt = 0;

			callCnt++; // Will overflow but is defined behaviour and should not cause problems (low collision probability)
                       // Also, name collision is no problem from the OS point of view (it's only for debugging)

			taskHandlersProtector.take();
			taskHandlers.push_back(task_t {std::bind(&me_t::fullHandler, this, std::placeholders::_1), parent_t::name + std::to_string(callCnt), parent_t::prio, std::move(evt)});
			// TODO : add bound checking
			taskHandlersProtector.give();

			// TEST ONLY :
			//fullHandler((void*)&evt);

			return true;
		}
		
		void fullHandler(Event<event_t>&& event) {
			
            parent_t::handlerFct(std::move(event));
            
            
			auto curHandle = task_t::currentHandle();
			auto toErase = std::remove_if(
			                   taskHandlers.begin(), taskHandlers.end(),
			[curHandle](task_t const & task) {
				return task.taskHandle == curHandle;
			}
			               );

			// Q : problem here ? task may be halted in the middle of the operation ?
			// A : No problem, FreeRTOS doesn't destroy right away, adds to destroy list and IdleTask destroys later
			taskHandlersProtector.take();
			taskHandlers.erase(toErase, taskHandlers.end());
			taskHandlersProtector.give();

			FFS::suspendCurrentTask();
		}
	};
}
