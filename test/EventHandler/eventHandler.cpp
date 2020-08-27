#include <iostream>
#include "RFF.h"

struct eventType { int myint; };

void handler(eventType const& mydata) {
    assert(mydata.myint == 8);
    RFF::OSStop();
}

RFF::EventHandler<eventType> evtHandler{handler};


void rff_main() {
    RFF::Mutex m;
    evtHandler(eventType{8});
}
