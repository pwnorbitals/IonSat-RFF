#pragma once

#include <list>
#include <tuple>
#include <memory>
#include <ctime>
#include <type_traits>
#include <any>

#include "module/module.h"
#include "OS/OS.h"
#include "RFF.h"

#include "unique_function.h"

namespace RFF {
    	
	
	/**
		\brief Controls the event flow

	*/
	class Controller {
	protected:
		unique_function<void (std::any) > emitter;
		unsigned int failCount;


	public:

		template<typename ...modules_t>
		Controller(modules_t& ..._modules) {

			// From : https://stackoverflow.com/questions/62652638
			emitter = [this, &_modules...](std::any && any_ev) mutable {      // invoked _modules' copy constructor which tries to copy the event handlers and the Tasks inside => fail.
				auto success = false;

				// Switch to generalized lambda capture from https://stackoverflow.com/questions/8640393
				auto dispatcher = [this, &any_ev, &_modules...](auto evtHandler) -> bool {
					using EventType = typename std::remove_pointer<decltype(evtHandler)>::type ::evt_t;
					if(any_ev.type() == typeid(EventType)) { // TODO : get rid of RTTI
						auto ev = std::any_cast<EventType> (any_ev); 
						((_modules.callHandlers(ev)), ...);
						return true;
					}

					return false;

				};
                
                 ((
                     std::apply([&dispatcher, &success](auto& ...handler){
                         if((dispatcher(handler) || ...)){
                             success = true;
                        }
                    }, _modules.evtHandlers)
                ), ...);

				if(!success) { failCount++; }
                    
			};
		}

		// TODO : make emit interrupt-safe
		template<typename evt_t>
		void emit(evt_t&& event) {
			emitter(std::move(event));
		}
	};
    
    
    extern Controller* ctrlr;
    
	/**
		\brief RFF Setup handler object
	*/
    template<typename ...modules_t>
    class Setup {
        Controller myController;
    public:
        
        Setup(modules_t& ..._modules) : myController{_modules...} {
            assert(ctrlr == nullptr);
            ctrlr = &myController;
        }
        ~Setup() = default;
        Setup(Setup const& other) = delete;
        Setup& operator=(Setup const& other) = delete;
    };
    
    
    template<typename evt_t>
    void emit(evt_t&& event) {
        assert(ctrlr != nullptr);
        ctrlr->emit(std::move(event));
    }
}
