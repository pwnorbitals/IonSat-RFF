#include "catch.hpp"

#define FFS_TEST

#include <iostream>
#include <thread>
#include "FFS.h"


TEST_CASE("Tasks", "[FreeRTOS]") {
    struct eventType{ int myint; };
    
    std::function<void(void*)> hdlr = [] (void* mydata) { 
        auto* evt = static_cast<FFS::Event<eventType>*>(mydata); 
        REQUIRE(evt->data.myint == 8);
        
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
    auto task = FFS::Task<eventType, 20>{hdlr, "test", FFS::Event{evt}, 1};
    FFS::OSStart();
    
}

TEST_CASE("Queues", "[FreeRTOS]") {
    
}

TEST_CASE("Semaphores", "[FreeRTOS]") {
    
}
