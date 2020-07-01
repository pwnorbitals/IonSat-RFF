#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <iostream>
#include "FFS.h"

TEST_CASE( "typical use case", "[example]" ) {
    
    struct testEvent{int evnum;};
    
    auto handler = [](FFS::Event<testEvent>* evt) { 
        
        std::cout << evt->data.evnum << std::endl; 
        // need to tell that I'm done ?!
    };
    auto eventHandler = FFS::EventHandler<testEvent>{handler, "testHandler", 256};
    auto module = FFS::Module{std::make_tuple(eventHandler)};
    auto modes = std::make_tuple(FFS::Mode{"test"});
    auto controller = FFS::Controller{FFS::OSSettings{}, modes, std::make_tuple(module)};
    
    controller.emit(testEvent{2});
    
    
    REQUIRE( 1 == 1 );
}
