#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include "FFS.h"



TEST_CASE("creation", "[EventHandler]") {
    
    std::function<void(void*)> hdlr = [] (void* mydata) { 
        auto* evt = static_cast<FFS::Event<int>*>(mydata); 
        REQUIRE(evt->data == 8);
        
        FFS::OSStop();
    };    
    
    struct eventType{ int data; };
    
    auto evtHandler = FFS::EventHandler<eventType, 1000000>{hdlr, "first", 1};
    auto evt = FFS::Event{eventType{8}, NULL};
    evtHandler(evt);
    
    FFS::OSStart();
    
}
