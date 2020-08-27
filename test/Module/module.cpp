
#include <iostream>
#include "RFF.h"

struct MyCustomEventType { int eventNo; };

void handler(MyCustomEventType const& evt) {
    assert(evt.eventNo == 42);
    std::cout << "Ok" << std::endl;
    RFF::OSStop();
}

RFF::EventHandler<MyCustomEventType, 1, 4, 1024> handler1{handler, "first"};
RFF::Module module{handler1};

 MyCustomEventType ev{42};

void rff_main() {
    module.callHandlers(ev);
}
