#define CATCH_CONFIG_MAIN
#include "catch.hpp"

int Test()
{
    return 2; 
}


TEST_CASE("default test", "[test]")
{
    REQUIRE(Test() == 2);
    REQUIRE(Test() != 3);
}