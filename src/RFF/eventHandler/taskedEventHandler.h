
#pragma once

#include "eventHandler.h"

namespace FFS {
	template<typename event_t, uint32_t stackDepth, uint16_t maxParallelHandlers>
	class TaskedEventHandler : public EventHandler<event_t, TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>>   {

        using me_t = TaskedEventHandler<event_t, stackDepth, maxParallelHandlers>;
		using parent_t = EventHandler<event_t, me_t>;

	protected:
		std::function<void(void*)> fullHandler;
		// boost::container::static_vector<Task<event_t, stackDepth>, maxParallelHandlers> taskHandlers;
		std::vector<Task<stackDepth, me_t*>> taskHandlers; // TODO : switch to static_vector
		Mutex taskHandlersProtector;
		Queue<Event<event_t>, maxParallelHandlers> waitingEvents;



	public:

        TaskedEventHandler() = delete;
        TaskedEventHandler(me_t const& other) = delete;
        me_t& operator=(me_t const& other) = delete;
        TaskedEventHandler(me_t&& other) : 
            parent_t{std::move(other)},
            taskHandlers{std::move(other.taskHandlers)},
            taskHandlersProtector{std::move(other.taskHandlersProtector)},
            waitingEvents{std::move(other.waitingEvents)}
            {};
        me_t& operator=(me_t&& other) {
            parent_t::operator=(std::move(other));
            taskHandlers = std::move(other.taskHandlers);
            taskHandlersProtector = std::move(other.taskHandlersProtector);
            waitingEvents = std::move(other.waitingEvents);
        }

		TaskedEventHandler(std::function<void(Event<event_t>)> _handlerFct, std::string _name, UBaseType_t _prio) :
			parent_t{_handlerFct, _name, _prio},
		fullHandler{[this](void* empty) {
            
            empty;

			Event<event_t> event{};
			auto res = waitingEvents.receive(event, 0);
			if(res != pdFALSE) {
				parent_t::handlerFct(event);
			}


			auto curHandle{Task<stackDepth, event_t>::currentHandle()};
			auto toErase = std::remove_if(
			                   taskHandlers.begin(), taskHandlers.end(),
			[curHandle](Task<stackDepth, me_t*> const & task) {
				return task.taskHandle == curHandle;
			}
			               );

			// Q : problem here ? task may be halted in the middle of the operation ?
			// A : No problem, FreeRTOS doesn't destroy right away, adds to destroy list and IdleTask destroys later
			taskHandlersProtector.take();
			taskHandlers.erase(toErase, taskHandlers.end());
			taskHandlersProtector.give();

			FFS::suspendCurrentTask();
		}},
		taskHandlers{}, taskHandlersProtector{}, waitingEvents{}
		{};


		bool handleEvent(Event<event_t> const& evt) {
			static uint16_t callCnt = 0;

			callCnt++; // Will overflow but is defined behaviour and should not cause problems (low collision probability)
			// Also, name collision is no problem from the OS point of view (it's only for debugging)

			auto res = waitingEvents.sendToBack(evt, 0);
			if(res == pdFALSE) { return false; }

			std::cout << "this ptr : " << this << std::endl;
			taskHandlersProtector.take();
			taskHandlers.push_back(Task<stackDepth, me_t*> {fullHandler, parent_t::name + std::to_string(callCnt), parent_t::prio});
			// TODO : add bound checking
			taskHandlersProtector.give();

			// TEST ONLY :
			//fullHandler((void*)&evt);

			return true;
		}
	};
}
