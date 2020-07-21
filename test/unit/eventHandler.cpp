#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include "FFS.h"


TEST_CASE( "Queued event handler" , "[EventHandler]" ){
    struct eventType{ int myint; };
    
    auto hdlr = [] (FFS::Event<eventType> const& mydata) { 
        REQUIRE(mydata.data.myint == 8);
        
        FFS::OSStop();
    };    
    
    
            
    auto evt = eventType{8}; 



    auto evtHandler = FFS::QueuedEventHandler<eventType, 1024, 64>{hdlr, "first", 1};
    evtHandler(evt);
    FFS::OSStart();
}

TEST_CASE( "Tasked event handler" , "[EventHandler]" ){
    struct eventType{ int myint; };
    
    auto hdlr = [] (FFS::Event<eventType> const& mydata) { 
        REQUIRE(mydata.data.myint == 8);
        
        FFS::OSStop();
    };    
    
    
            
    auto evt = eventType{8}; 



    auto evtHandler = FFS::TaskedEventHandler<eventType, 1024, 64>{hdlr, "first", 1};
    evtHandler(evt);
    FFS::OSStart();
}
