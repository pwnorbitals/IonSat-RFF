#include <iostream>
#include "RFF.h"

struct MyCustomEventType { int eventNo; };

void handler(MyCustomEventType const& evt) {
    assert(evt.eventNo == 42);
    RFF::OSStop();
}

RFF::EventHandler<MyCustomEventType, 1, 8, 1024> handler1{handler, "first"};
RFF::Module module{handler1};
RFF::Controller controller{module};

void rff_main() {
	controller.emit(MyCustomEventType{42});
}
