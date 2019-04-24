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

TEST_CASE("test first alignment from reads", "[Alignment]")
{
    Sequence g = {"G_substring", "AGTGAAGTCAGGATAAAACTTATAAGAACAAATGCCCTTTACGAGCCAAAGAGAGCTACATCAATGAACTGTAATGAGCAGCCACGTTCCAGTCTTATGTTTCAACTAAAGCCATGAATGTCTCATGATACAAATAAAACAGTACCCGCAGTCTCAAATATTTAGCCTAAGTTGCATAAAAAGTTGGCTTCCATAATGAG"};
    Sequence r = {"read1", "CAACTAAAGCCATGAATGTCTAATGATACAAATAAGACAGTACCCGCAGTCTCAAATATTTAGCCTAAGTTGCATAACAAGTTGGCTTCCATAATGAGAGACT"};
    Alignment alignment(r, g);
    alignment.doLocalAlignment();
    AlignmentData aData = alignment.getData();
    cout << "Highest score: " << aData.max_score << "\n";
    cout << "Location: " << aData.bestHit_j0 << "-" << aData.betsHit_j1 << std::endl;
    cout << "Percent identity: " << aData.percentIdentity << std::endl;
    cout << "Length coverage: " << aData.lengthCoverage << std::endl;
    REQUIRE(aData.percentIdentity > 90);
}