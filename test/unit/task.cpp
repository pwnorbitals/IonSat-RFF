
#define FFS_TEST

#include <iostream>
#include <thread>
#include "FFS.h"


void ffs_main() {
	struct eventType { int myint; };

	void(*hdlr)(FFS::Event<eventType>*) = [](FFS::Event<eventType>* evt) {
		assert(evt->data.myint == 8);

		/*
		SECTION( "priority changes" ){
		    REQUIRE(task.priority() == 1);

		    task.priority(3);
		    REQUIRE(task.priority() == 3);
		}

		SECTION( "suspend and resume" ) {
		    task.suspend();
		    REQUIRE(1 == 1); // TODO
		    task.resume();
		    REQUIRE(1 == 1); // TODO
		}


		SECTION( "delays" ){
		    task.delay();     // TODO
		    REQUIRE(1 == 1); // TODO

		    SECTION( "abort delay" ){
		        task.abortDelay(); // TODO
		        REQUIRE(1 == 1); // TODO
		    }

		    task.delayUntil(); // TODO
		    REQUIRE(1 == 1); // TODO
		}
		*/

		FFS::OSStop();
	};
	auto evt = FFS::Event{eventType{8}};
	auto task = FFS::Task<20000> {(void(*)(void*))hdlr, "test", 1, &evt};
	FFS::suspendCurrentTask();

}
