#include <iostream>
#include "RFF.h"

#include "ctti/type_id.hpp"


struct ModuleStub {
    RFF::Mutex& m;

    ModuleStub(RFF::Mutex& _m) : m{_m} {
        m.take();
    }
    
    void callHandlers(const void* value, ctti::type_id_t type) {
        // TODO : asserts
        m.give();
    }

    void callHandlersISR(const void* value, ctti::type_id_t type) {
        // TODO : asserts
        m.give();
    }
};


RFF::Mutex checker1{}; RFF::Mutex checker2{};
ModuleStub stub1{checker1}; ModuleStub stub2{checker2};


void rff_main() {
    RFF::Controller controller{stub1, stub2};
	controller.emit(42);
    checker1.take(portMAX_DELAY);
    checker2.take(portMAX_DELAY);

    RFF::OSStop();
}
