
#include <iostream>
#include "FFS.h"

void ffs_main() {
	struct MyCustomEventType { int eventNo; };

	void(*handler)(MyCustomEventType const&) = [](MyCustomEventType const& evt) {
		std::cout << "ok : " << evt.eventNo << std::endl;
		assert(evt.eventNo == 42);
		FFS::suspendCurrentTask();
	};



	auto handler1 = FFS::EventHandler<MyCustomEventType, 1, 64, 1000000> {handler, "first"};
	auto module = FFS::Module{handler1};
    
    auto ev = MyCustomEventType{42};
    
    module.callHandlers(ev);
    
	FFS::suspendCurrentTask();
}
