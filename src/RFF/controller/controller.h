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

template<bool...> struct bool_pack;
template<bool... bs> using all_true = std::is_same<bool_pack<bs..., true>, bool_pack<true, bs...>>;
template<class R, class ...Ts> using are_all_different = all_true<std::negation_v<std::is_same_v<Ts, R>...>>;


namespace RFF {

	
    	
	
	/**
		\brief Controls the event flow

	*/
	
	class Controller {
	protected:
		std::function<void(const void* value, ctti::type_id_t type)> emitter;


	public:

		Controller() = delete;
		Controller(Controller const& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(Controller const& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		template<typename ...modules_t>
		Controller(modules_t& ..._modules)  {
			emitter = [_modules...](const void* value, ctti::type_id_t type) mutable {
				(_modules.callHandlers(value, type), ...);
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
