#include "catch.hpp"
#include "../SuffixTree/SuffixTree.h"
#include "../SuffixTree/SuffixTreeNode.h"


void BuildSuffixTree()
{
    SuffixTree st();
}


TEST_CASE("test building suffix tree does not throw exceptions", "[SuffixTree]")
{
    REQUIRE_NOTHROW(BuildSuffixTree());
}

TEST_CASE("Test that the nodes have initalized their start and end leafs to zero", "[SuffixTree]")
{
    SuffixTreeNode node = SuffixTreeNode();

    REQUIRE(node.get_start_leaf_index() == -1);
    REQUIRE(node.get_end_leaf_index() == -1);
}