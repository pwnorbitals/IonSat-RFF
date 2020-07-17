#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include "FFS.h"



TEST_CASE("creation", "[EventHandler]") {
    
    struct eventType{ int myint; };
    
    std::function<void(FFS::Event<eventType>*)> hdlr = [] (FFS::Event<eventType>* mydata) { 
        REQUIRE(mydata->data.myint == 8);
        
        FFS::OSStop();
    };    
    
    auto evt = FFS::Event{eventType{8}};   
    auto evtHandler = FFS::QueuedEventHandler<eventType, 1000000, 64>{hdlr, "first", 1};
    evtHandler(evt);
    
    FFS::OSStart();
    
}
