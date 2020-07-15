
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include "FFS.h"

TEST_CASE("", "[Module]") {
    std::cout << "ok" << std::endl;
    REQUIRE(1 == 1);
}
