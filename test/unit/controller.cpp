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




	auto handlers = std::make_tuple(FFS::TaskedEventHandler<MyCustomEventType, 1000000, 64> {handler, "first", 1});
	auto modules = std::make_tuple(FFS::Module{std::move(handlers)});
	auto controller = FFS::Controller{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"abc"}), std::move(modules), event_tags{}};

	controller.emit(MyCustomEventType{42});
	FFS::suspendCurrentTask();
}
