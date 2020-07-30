
#define FFS_TEST

#include <iostream>
#include <thread>
#include "FFS.h"


void ffs_main() {
	struct eventType { int myint; };

	void(*hdlr)(eventType const&) = [](eventType const& evt) {
		assert(evt.myint == 8);

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
	auto evt = eventType{8};
	auto task = FFS::Task<1, 20000> {(void(*)(void*))hdlr, "test", &evt};
	FFS::suspendCurrentTask();

}
