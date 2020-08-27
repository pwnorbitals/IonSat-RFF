#pragma once

#include <list>
#include <tuple>
#include <memory>
#include <ctime>
#include <type_traits>
#include <any>
#include <variant>

#include "ctti/type_id.hpp"

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
		std::function<void(const void* value, ctti::type_id_t type)> emitter;


	public:

		template<typename ...modules_t>
		Controller(modules_t& ..._modules)  {
			emitter = [_modules...](const void* value, ctti::type_id_t type) {
				std::apply([&](auto& ...module) {
					(module.callHandlers(value, type), ...);
				}, std::make_tuple(_modules...));
			};
		}

		// TODO : make emit interrupt-safe
		template<typename evt_t>
		void emit(evt_t const& event) {
			emitter(&event, ctti::type_id<evt_t>());
		}

		virtual ~Controller() = default;
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
