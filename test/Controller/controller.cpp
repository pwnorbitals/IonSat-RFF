#include <iostream>
#include "RFF.h"

#include "ctti/type_id.hpp"

struct MyCustomEventType { int eventNo; };

struct ModuleStub {
    RFF::Mutex& m;

    ModuleStub(RFF::Mutex _m) : m{_m} {
        m.take();
    }
    
    void callHandlers(void* value, ctti::type_id_t type) {
        // TODO : asserts
        m.give();
    }
};

RFF::Mutex checker1{}; RFF::Mutex checker2{};
ModuleStub stub1{checker1}; ModuleStub stub2{checker2};
RFF::Controller controller{stub1, stub2};

void rff_main() {
	controller.emit(MyCustomEventType{42});
    checker1.take();
    checker2.take();

    RFF::Controller controller2{controller}; // copy constructor
    checker1.take();
    checker2.take();

    RFF::Controller controller3{std::move(controller)}; // move constructor
    checker1.take();
    checker2.take();

    RFF::Controller controller4; controller4 = controller3; // copy assignment
    checker1.take();
    checker2.take();

    RFF::Controller controller5; controller5 = std::move(controller3); // move assignment
    checker1.take();
    checker2.take();

    RFF::OSStop();
}
