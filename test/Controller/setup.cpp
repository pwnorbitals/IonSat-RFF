#include <iostream>
#include "RFF.h"

#include "ctti/type_id.hpp"

struct EventThatShouldFail{int i;};


struct ModuleStub {
    RFF::Mutex& m;

    ModuleStub(RFF::Mutex& _m) : m{_m} {
        m.take(portMAX_DELAY);
    }
    
    void callHandlers(const void* value, ctti::type_id_t type) {
        assert(*(int*)value == 42);
        assert(type == ctti::type_id<int>());
        m.give();
    }

    void callHandlersISR(const void* value, ctti::type_id_t type) {
        assert(*(int*)value == 42);
        assert(type == ctti::type_id<int>());
        m.give();
    }

    constexpr bool hasHandler(ctti::type_id_t const type) {
        return true; // TODO
    }
};


RFF::Mutex checker1{}; RFF::Mutex checker2{};
ModuleStub stub1{checker1}; ModuleStub stub2{checker2};


void rff_main() {
    RFF::Setup setup{stub1, stub2};
	RFF::emit(42);
    checker1.take(portMAX_DELAY);
    checker2.take(portMAX_DELAY);
    // RFF::emit(EventThatShouldFail{42});

    RFF::OSStop();
}
