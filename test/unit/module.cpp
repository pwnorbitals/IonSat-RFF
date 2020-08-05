
#include <iostream>
#include "FFS.h"

struct MyCustomEventType { int eventNo; };

void handler(MyCustomEventType const& evt) {
    assert(evt.eventNo == 42);
    std::cout << "Ok" << std::endl;
    FFS::OSStop();
}

FFS::EventHandler<MyCustomEventType, 1, 4, 1024> handler1{handler, "first"};
FFS::Module module{handler1};

 MyCustomEventType ev{42};

void ffs_main() {
    module.callHandlers(ev);
}
