#include <iostream>
#include "FFS.h"

struct MyCustomEventType { int eventNo; };

void handler(MyCustomEventType const& evt) {
    assert(evt.eventNo == 42);
    std::cout << "Ok" << std::endl;
    FFS::OSStop();
}

FFS::EventHandler<MyCustomEventType, 1, 64, 100000> handler1{handler, "first"};
FFS::Module module{handler1};
FFS::Controller controller{std::make_tuple(FFS::Mode{"abc"}), module};

void ffs_main() {
	controller.emit(MyCustomEventType{42});
}
