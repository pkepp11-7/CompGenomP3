#include "catch.hpp"
#include "../SuffixTree/SuffixTree.h"


void BuildSuffixTree()
{
    SuffixTree st();
}


TEST_CASE("test building suffix tree does not throw exceptions", "[SuffixTree]")
{
    REQUIRE_NOTHROW(BuildSuffixTree());
}