#include <iostream>
#include "FFS.h"

void ffs_main() {
	struct MyCustomEventType { int eventNo; };

	using event_tags = std::tuple<FFS::Tag<MyCustomEventType>>;


	auto handler = [](FFS::Event<MyCustomEventType>* evt) {
		std::cout << "ok : " << evt->data.eventNo << std::endl;
		assert(evt->data.eventNo == 42);
		FFS::suspendCurrentTask();
	};




	auto handler1 = FFS::TaskedEventHandler<MyCustomEventType, 1000000, 64> {handler, "first", 1};
	auto module = FFS::Module{handler1};
	auto controller = FFS::Controller{std::make_tuple(FFS::Mode{"abc"}), event_tags{}, module};

	controller.emit(MyCustomEventType{42});
	FFS::suspendCurrentTask();
}
