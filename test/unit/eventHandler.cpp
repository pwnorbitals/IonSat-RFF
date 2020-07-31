#define FFS_TEST

#include <iostream>
#include "FFS.h"


void ffs_main() {
	struct eventType { int myint; };

	auto hdlr = [](eventType const& mydata) {
		assert(mydata.myint == 8);
        std::cout << "Ok" << std::endl;
		FFS::OSStop();
	};



	auto evt = eventType{8};

    
    auto evtHandler = FFS::EventHandler<eventType, 1> {hdlr, "first"};
	evtHandler(std::move(evt));
	FFS::suspendCurrentTask();
}

/*
TEST_CASE( "Tasked event handler" , "[EventHandler]" ){
    struct eventType{ int myint; };

    auto hdlr = [] (FFS::Event<eventType> const& mydata) {
        REQUIRE(mydata.data.myint == 8);

        FFS::OSStop();
    };



    auto evt = FFS::Event{eventType{8}};



    auto evtHandler = FFS::TaskedEventHandler<eventType, 1024, 64>{hdlr, "first", 1};
    evtHandler(evt);
    FFS::OSStart();
}
*/
