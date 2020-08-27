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
};

RFF::Mutex checker1{}; RFF::Mutex checker2{};
ModuleStub stub1{checker1}; ModuleStub stub2{checker2};


void rff_main() {
    RFF::Controller controller{stub1, stub2};
	controller.emit(42);
    checker1.take();
    checker2.take();

    RFF::Controller controller2{controller}; // copy constructor
    controller2.emit(42);
    checker1.take();
    checker2.take();

    RFF::Controller controller3{std::move(controller)}; // move constructor
    controller3.emit(42);
    checker1.take();
    checker2.take();

    RFF::Controller controller4; controller4 = controller3; // copy assignment
    controller4.emit(42);
    checker1.take();
    checker2.take();

    RFF::Controller controller5; controller5 = std::move(controller3); // move assignment
    controller5.emit(42);
    checker1.take();
    checker2.take();

    RFF::OSStop();
}
