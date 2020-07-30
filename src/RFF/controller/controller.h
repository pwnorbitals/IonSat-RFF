#pragma once

#include <list>
#include <tuple>
#include <memory>
#include <ctime>
#include <type_traits>
#include <any>

#include "mode/mode.h"
#include "module/module.h"
#include "OS/OS.h"
#include "FFS.h"

#include "unique_function.h"

namespace FFS {
    	

	class Controller {
	protected:
		std::tuple<FFS::Mode> modes;
		unique_function<void (std::any) > func;


	public:

		template<typename ...modules_t>
		Controller(std::tuple<FFS::Mode> _modes, modules_t& ..._modules) : modes{_modes} {

			// From : https://stackoverflow.com/questions/62652638
			func = [this, &_modules...](std::any && any_ev) mutable {      // invoked _modules' copy constructor which tries to copy the event handlers and the Tasks inside => fail.
				auto success = false;

				// Switch to generalized lambda capture from https://stackoverflow.com/questions/8640393
				auto f = [this, &any_ev, &_modules...](auto evtHandler) -> bool {
					using EventType = typename std::remove_pointer<decltype(evtHandler)>::type ::evt_t;
					if(any_ev.type() == typeid(EventType)) { // TODO : get rid of RTTI
						auto ev = std::any_cast<EventType> (any_ev); 
						((_modules.callHandlers(ev)), ...);
						return true;
					}

					return false;

				};
                
                 ((
                     std::apply([&f, &success](auto& ...h){
                         if((f(h) || ...)){
                             success = true;
                        }
                    }, _modules.evtHandlers)
                ), ...);
                    
				if(!success) {
					std::cout << "no match :( " << std::endl;
				} else {
					std::cout << "matched" << std::endl;
				}
			};
		}

		// TODO : make emit interrupt-safe
		template<typename evt_t>
		void emit(evt_t&& event) {
			func(std::move(event));
		}
	};
    
    
    extern Controller* ctrlr;
    
    template<typename ...modules_t>
    class Setup {
        Controller myController;
    public:
        
        Setup(std::tuple<FFS::Mode> _modes, modules_t& ..._modules) : myController{_modes, _modules...} {
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
