#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "FFS.h"

TEST_CASE( "Controller initialization works", "[controller]" ) {
    
    auto controller = FFS::Controller{};
    
    REQUIRE( 1 == 1 );
}
