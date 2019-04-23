#include "catch.hpp"
#include "../SuffixTree/SuffixTree.h"
#include "../SuffixTree/SuffixTreeNode.h"
#include "../Alphabet.h"
#include "../FileReader/fastafilereader.h"


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
        REQUIRE(readIndex == 0);
    }
    SECTION("ending in the middle of a string")
    {
        string read = string("ba");
        expected = {5,8};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        REQUIRE(expected[0] == actual[0]);
        REQUIRE(expected[1] == actual[1]);
        REQUIRE(readIndex == 0);
    }
    SECTION("going through multipule nodes ending at a node")
    {
        string read = string("acc");
        expected = {9};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        REQUIRE(expected[0] == actual[0]);
        REQUIRE(readIndex == 2);
    }
    SECTION("going through multipule nodes ending at an edge")
    {
        string read = string("acac");
        expected = {1};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        for(int i = 0; i < expected.size(); i++)
        {
            REQUIRE(expected[0] == actual[0]);
        }
        REQUIRE(readIndex == 2);
    }
    SECTION("going through multipule nodes ending at an edge with internal node children")
    {
        string read = string("acba");
        expected = {3,6};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        for(int i = 0; i < expected.size(); i++)
        {
            REQUIRE(expected[0] == actual[0]);
        }
        REQUIRE(readIndex == 2);
    }
    SECTION("mismatch on edge")
    {
        string read = string("baa");
        expected = {5,8};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        for(int i = 0; i < expected.size(); i++)
        {
            REQUIRE(expected[0] == actual[0]);
        }
        REQUIRE(readIndex == 2);
    }
    SECTION("using a suffix link")
    {
        string read = string("bacac");
        expected = {1};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        for(int i = 0; i < expected.size(); i++)
        {
            REQUIRE(expected[0] == actual[0]);
        }
        REQUIRE(readIndex == 3);
    }
    SECTION("Mismatch after a node")
    {
        string read = string("cbaca");
        expected = {4,7};
        actual = st.findLocation(readIndex, &read);

        REQUIRE(expected.size() == actual.size());
        for(int i = 0; i < expected.size(); i++)
        {
            REQUIRE(expected[0] == actual[0]);
        }
        REQUIRE(readIndex == 4);
    }
}

TEST_CASE("real world example", "[SuffixTree]")
{
    //doesnt need to be 100 percent acurate just close
    
    //>READ_329422_from_Peach_reference.fasta_AT_3921681
    //AATTTGGTGATTCCAGTGTTAACGGTGAACTGTACCTTTAAAGAATCTCTGAGACCAATGTCTTTTGCCCGATTGATTGTTCCTGCTGCCTATGTGATGAGC
    string readInput = string("AATTTGGTGATTCCAGTGTTAACGGTGAACTGTACCTTTAAAGAATCTCTGAGACCAATGTCTTTTGCCCGATTGATTGTTCCTGCTGCCTATGTGATGAGC");
    int readIndex = 0;
    fstream reader;
    
    reader.open("../InputFiles/PeachInput.fasta", fstream::in);
    FastaFileReader fReader = FastaFileReader(&reader);

    string peach = fReader.getNextSequence().nucleotideSequence;
    peach.append("$");

    Alphabet::createAlphabet("ACGT");
    STData::init(& peach, peach.length());

    SuffixTree st = SuffixTree();
    st.McCreightInsert(&peach);
    st.DFS();

    vector<int> actual_locations = st.findLocation(readIndex, &readInput);

    int closeEnough = 0;

    for(int i = 0; i < actual_locations.size(); i++)
    {
        cout << i << ": " << actual_locations[i] << "\n";
        if(3921681 - 25 < actual_locations[i] && actual_locations[i] < 3921681 + 25)
        {
            closeEnough = 1;
        }
    }

    REQUIRE(closeEnough == 1);
    
}