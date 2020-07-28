
#define FFS_TEST

#include <iostream>
#include "FFS.h"


void ffs_main() {

	struct MyCustomEventType { int eventNo; };
	struct MyOtherCustomEventType { bool sent; };


	void(*handler)(FFS::Event<MyCustomEventType>*) = [](FFS::Event<MyCustomEventType>* evt) {
		std::cout << "YAY ! got " << evt->data.eventNo << std::endl;
		assert(evt->data.eventNo == 42);

		evt->controller->emit(MyOtherCustomEventType{true});
	};



	void(*otherhandler)(FFS::Event<MyOtherCustomEventType>*) = [](FFS::Event<MyOtherCustomEventType>* evt) {
		std::cout << "boom !! that's " << evt->data.sent << std::endl;
		assert(evt->data.sent == true);

		FFS::OSStop();
	};



    auto h1 = FFS::QueuedEventHandler<MyCustomEventType, 100000, 64> {handler, "first", 1};
    auto h2 = FFS::TaskedEventHandler<MyOtherCustomEventType, 100000, 64> {otherhandler, "second", 2};
    auto m1 = FFS::Module{h1, h2};

	auto controller = FFS::Controller{std::make_tuple(FFS::Mode{"abc"}), m1};

	controller.emit(MyCustomEventType{42});

	FFS::suspendCurrentTask();

}

/*
TEST_CASE( "typical use case", "[example]" ) {

}
*/

