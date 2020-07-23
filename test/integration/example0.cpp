
#define FFS_TEST

#include <iostream>
#include "FFS.h"


void ffs_main() {

	struct MyCustomEventType { int eventNo; };
	struct MyOtherCustomEventType { bool sent; };

	using event_tags = std::tuple<FFS::Tag<MyCustomEventType>, FFS::Tag<MyOtherCustomEventType>>;
	// using events = FFS::declare_events<myCustomEventType, MyOtherCustomEventType>


	void(*handler)(FFS::Event<MyCustomEventType>*) = [](FFS::Event<MyCustomEventType>* evt) {
		std::cout << "YAY ! got " << evt->data.eventNo << std::endl;
		assert(evt->data.eventNo == 42);

		evt->controller->emit(MyOtherCustomEventType{true});
	};



	void(*otherhandler)(FFS::Event<MyOtherCustomEventType>*) = [](FFS::Event<MyOtherCustomEventType>* evt) {
		std::cout << "boom !! that's" << evt->data.sent << std::endl;
		assert(evt->data.sent == true);

		FFS::OSStop();
	};





	auto handlers = std::make_tuple(FFS::QueuedEventHandler<MyCustomEventType, 100000, 64> {handler, "first", 1},
	                                FFS::TaskedEventHandler<MyOtherCustomEventType, 100000, 64> {otherhandler, "second", 2});


	auto modules = std::make_tuple(FFS::Module{std::move(handlers)});

	auto controller = FFS::Controller{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"abc"}), std::move(modules), event_tags{}};

	controller.emit(MyCustomEventType{42});

	FFS::suspendCurrentTask();

}

/*
TEST_CASE( "typical use case", "[example]" ) {

}
*/

