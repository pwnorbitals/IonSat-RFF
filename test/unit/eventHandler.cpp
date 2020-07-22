#define FFS_TEST

#include <iostream>
#include "FFS.h"


void ffs_main() {
	struct eventType { int myint; };

	auto hdlr = [](FFS::Event<eventType> const & mydata) {
		assert(mydata.data.myint == 8);

		FFS::OSStop();
	};



	auto evt = FFS::Event{eventType{8}};



	auto evtHandler = FFS::QueuedEventHandler<eventType, 1024, 64> {hdlr, "first", 1};
    auto evtHandler2 = FFS::QueuedEventHandler{std::move(evtHandler)}; // check move works
    
    
    
	evtHandler2(std::move(evt));
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
