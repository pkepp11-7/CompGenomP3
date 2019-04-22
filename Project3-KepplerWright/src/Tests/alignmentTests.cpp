#include "catch.hpp"
#include "../Alignment/Alignment.h"

TEST_CASE("test running local alignment does not throw exceptions", "[Alignment]")
{
    Sequence s1 = {"s1", "ACGT"};
    Sequence s2 = {"s2", "CGT"};
    Alignment alignment(s1,s2);
    REQUIRE_NOTHROW(alignment.doLocalAlignment());
}

TEST_CASE("test running alignment produces correct number of identites", "[Alignment]")
{
    //test case from project 1 website
    Sequence s1 = {"s1", "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT"};
    Sequence s2 = {"s2", "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT"};
    Alignment alignment(s1,s2, NULL, 1, -2, -5, -2);
    alignment.doGlobalAlignment();
    AlignmentData aData = alignment.getData();
    REQUIRE(aData.percentIdentity == 84);
}