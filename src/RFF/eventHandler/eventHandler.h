#ifndef EVENTHANDLER_H_INCLUDED
#define EVENTHANDLER_H_INCLUDED


#include "event/event.h"
#include <functional>
#include <boost/container/static_vector.hpp>
#include <iostream>
#include <type_traits>


namespace FFS {
	template<typename event_t, uint32_t stackDepth>
	class EventHandler {


	protected:

		std::function<void (Event<event_t>*) > handlerFct;
		// boost::container::static_vector<Task<event_t, stackDepth>, FFS_MAX_PARALLEL_HANDLERS> taskHandlers;
		std::vector<Task<event_t, stackDepth>> taskHandlers;
		std::string name;
		UBaseType_t prio;






	public:

		EventHandler() = delete;
		EventHandler(EventHandler const& other) = delete;
		EventHandler& operator= (EventHandler const& other) = delete;
		EventHandler(EventHandler&& other) = default;
		EventHandler& operator= (EventHandler&& other) = default;

		EventHandler(std::function<void (Event<event_t>*) > _handlerFct, std::string _name, UBaseType_t _prio) :
			handlerFct{_handlerFct}, taskHandlers{}, name{_name}, prio(_prio) {

		}

		template<typename evt_t>
		void operator()(Event<evt_t> const& evt) {
			if constexpr(std::is_same<evt_t, event_t>::value) {
				std::cout << "pushing back" << std::endl;

				auto cleanup = [ = ]() {
					std::cout << "cleaning up" << std::endl;
					std::remove_if(
					    taskHandlers.begin(), taskHandlers.end(),
					[&](FFS::Task<event_t, stackDepth> const & task) {
						return task.event == evt;
					}
					);

				};

				std::function<void (void*) > hf = [&](void* arg) {
					// dangerous ! but needed for interoperability with FreeRTOS' void(*)(void*) thread function
					std::cout << "casting fct pointer" << std::endl;
					auto event = reinterpret_cast<FFS::Event<event_t>*>(arg);

					std::cout << "handling event" << std::endl;
					handlerFct(event);

					std::cout << "cleaning up" << std::endl;
					cleanup();
				};

				taskHandlers.push_back(FFS::Task<event_t, stackDepth> {std::move(hf), name.c_str(), evt, prio});
				std::cout << "done pushing back" << std::endl;
			}
		}


	};
}

#endif
