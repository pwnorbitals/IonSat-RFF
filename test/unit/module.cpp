
#include <iostream>
#include "FFS.h"

void ffs_main() {
	struct MyCustomEventType { int eventNo; };

	using event_tags = std::tuple<FFS::Tag<MyCustomEventType>>;


	auto handler = [](FFS::Event<MyCustomEventType> const & evt) {
		std::cout << "ok : " << evt.data.eventNo << std::endl;
		assert(evt.data.eventNo == 42);
		FFS::suspendCurrentTask();
	};



	auto handlers = std::make_tuple(FFS::TaskedEventHandler<MyCustomEventType, 1000000, 64> {handler, "first", 1});
	auto module = FFS::Module{std::move(handlers)};
    
    auto ev = MyCustomEventType{42};
    auto full_ev = FFS::Event{ev};
    
    module.callHandlers(full_ev);
    
	FFS::suspendCurrentTask();
}
