#include <iostream>
#include "FFS.h"

void ffs_main() {
	struct MyCustomEventType { int eventNo; };

	auto handler = [](MyCustomEventType const& evt) {
		std::cout << "ok : " << evt.eventNo << std::endl;
		assert(evt.eventNo == 42);
		FFS::suspendCurrentTask();
	};




	auto handler1 = FFS::EventHandler<MyCustomEventType, 1, 64, 100000> {handler, "first"};
	auto module = FFS::Module{handler1};
	auto controller = FFS::Controller{std::make_tuple(FFS::Mode{"abc"}), module};

	controller.emit(MyCustomEventType{42});
	FFS::suspendCurrentTask();
}
