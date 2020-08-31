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
    
    static_assert(!std::is_copy_constructible_v<RFF::EventHandler<eventType>>);
    static_assert(!std::is_copy_assignable_v<RFF::EventHandler<eventType>>);
    static_assert(!std::is_move_constructible_v<RFF::EventHandler<eventType>>);
    static_assert(!std::is_move_assignable_v<RFF::EventHandler<eventType>>);

}
