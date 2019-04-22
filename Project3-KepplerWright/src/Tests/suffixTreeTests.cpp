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

TEST_CASE("Test get location", "[SuffixTree]")
{
    vector<int> expected;
    vector<int> actual;
    int readIndex = 0;
    string StInput("acacbacbacc$");

    Alphabet::createAlphabet("abc");
    STData::init(& StInput, StInput.length());

    SuffixTree st = SuffixTree();
    st.McCreightInsert(&StInput);
    st.DFS();

    SECTION("basic findlocation ending at node")
    {
        string read = string("bac");
        expected = {5,8};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        REQUIRE(expected[0] == actual[0]);
        REQUIRE(expected[1] == actual[1]);
        REQUIRE(readIndex == 3);
    }
    SECTION("ending in the middle of a string")
    {
        string read = string("ba");
        expected = {5,8};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        REQUIRE(expected[0] == actual[0]);
        REQUIRE(expected[1] == actual[1]);
        REQUIRE(readIndex == 2);
    }
    SECTION("going through multipule nodes ending at a node")
    {
        string read = string("acc");
        expected = {9};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        REQUIRE(expected[0] == actual[0]);
        REQUIRE(readIndex == 3);
    }
    SECTION("going through multipule nodes ending at an edge")
    {
        string read = string("acac");
        expected = {1,3,6,9};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        for(int i = 0; i < expected.size(); i++)
        {
            REQUIRE(expected[0] == actual[0]);
        }
        REQUIRE(readIndex == 4);
    }
}