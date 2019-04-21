#include "catch.hpp"
#include "../SuffixTree/SuffixTree.h"
#include "../SuffixTree/SuffixTreeNode.h"
#include "../Alphabet.h"


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

TEST_CASE("Test that leaf array was created proerly", "[SuffixTree]")
{
    string input = string("GATAGACA$");
    int expectedLeafArray[input.size()] = {9, 8, 6, 4, 2, 7, 5, 1, 3};
    int* actualLeafArray;

    Alphabet::createAlphabet("ACGT");
    STData::init(& input, input.length());

    SuffixTree st = SuffixTree();
    st.McCreightInsert(&input);
    st.DFS();

    actualLeafArray = st.getSuffixTreeLeafArray();

    //assert
    for(int i = 0; i < input.size(); i++)
    {
        REQUIRE(expectedLeafArray[i] == actualLeafArray[i]);
    }
}