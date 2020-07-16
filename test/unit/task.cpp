#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include <thread>
#include "FFS.h"


TEST_CASE("Tasks", "[FreeRTOS]") {
    int evtdata = 8;
    std::function<void(void*)> hdlr = [] (void* mydata) { 
        auto* evt = static_cast<FFS::Event<int>*>(mydata); 
        REQUIRE(evt->data == 8);
        
        FFS::OSStop();
    };
    auto task = FFS::Task<int, 20>{hdlr, "test", FFS::Event{evtdata, NULL}, 1};
    FFS::OSStart();
    
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
}

TEST_CASE("Queues", "[FreeRTOS]") {
    
}

TEST_CASE("Semaphores", "[FreeRTOS]") {
    
}
