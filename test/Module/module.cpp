
#include <iostream>
#include "RFF.h"
#include "ctti/type_id.hpp"

template<typename T>
struct handlerStub {
    using evt_t = T;
    RFF::Mutex& m;

    handlerStub(RFF::Mutex& _m) : m{_m}{
        m.take(portMAX_DELAY);
    }

    void operator()(T event) { 
        assert(event == 42);
        m.give();
    }

};

RFF::Mutex checker1{}; 
RFF::Mutex checker2{};

handlerStub<int> h1{checker1};
handlerStub<float> h2{checker2};

RFF::Module module{h1, h2};


void rff_main() {
    auto toCall = 42;

    module.callHandlers(&toCall, ctti::type_id<decltype(toCall)>());
    RFF::OSStop();
}
