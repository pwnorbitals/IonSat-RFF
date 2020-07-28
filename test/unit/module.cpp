
#include <iostream>
#include "FFS.h"

void ffs_main() {
	struct MyCustomEventType { int eventNo; };

	using event_tags = std::tuple<FFS::Tag<MyCustomEventType>>;


	void(*handler)(FFS::Event<MyCustomEventType>*) = [](FFS::Event<MyCustomEventType>* evt) {
		std::cout << "ok : " << evt->data.eventNo << std::endl;
		assert(evt->data.eventNo == 42);
		FFS::suspendCurrentTask();
	};



	auto handler1 = FFS::TaskedEventHandler<MyCustomEventType, 1000000, 64> {handler, "first", 1};
	auto module = FFS::Module{handler1};
    
    auto ev = MyCustomEventType{42};
    auto full_ev = FFS::Event{ev};
    
    module.callHandlers(full_ev);
    
	FFS::suspendCurrentTask();
}
