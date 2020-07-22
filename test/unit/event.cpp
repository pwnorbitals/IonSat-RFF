#include <iostream>
#include "FFS.h"

struct eventType { int myint; };
bool operator==(eventType const& lhs, eventType const& rhs) {return lhs.myint == rhs.myint;}
bool operator!=(eventType const& lhs, eventType const& rhs) {return lhs.myint != rhs.myint;}

void ffs_main() {


	auto start = std::clock();
	auto evt = FFS::Event{eventType{8}};
	/*
	SECTION( "Construction" ){
	    REQUIRE(evt.controller == NULL);
	    REQUIRE(evt.data == eventType{8});
	    REQUIRE(evt.data.myint == 8);
	    REQUIRE((int)(evt.emissionTime/100) == (int)(start/100));
	    // REQUIRE(evt.sha1 == );
	}

	SECTION( "Operator==" ){
	        auto otherEvt = FFS::Event{eventType{8}};
	        auto badEvt = FFS::Event{int{4}};
	        REQUIRE(evt != otherEvt); // They are equivalent, but not the same
	        REQUIRE(evt != badEvt);
	    }
	    */

	FFS::suspendCurrentTask();
}
