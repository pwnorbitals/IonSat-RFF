#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "FFS.h"

TEST_CASE( "Controller initialization works", "[controller]" ) {
    
    struct testEvent{int evnum;};
    
    auto handler = [](std::unique_ptr<FFS::Event<testEvent>> evt) { };
    auto module = FFS::Module{std::make_tuple(FFS::EventHandler<testEvent>{handler, "testHandler", 256})};
    auto controller = FFS::Controller<testEvent>{FFS::OSSettings{}, std::make_tuple(FFS::Mode{"test"}), std::make_tuple(module)};
    
    REQUIRE( 1 == 1 );
}
