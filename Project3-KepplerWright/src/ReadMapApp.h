//Patrick Keppler and Connor Wright, 2019
#ifndef READMAPAPP_H
#define READMAPAPP_H

#include "Alignment/Alignment.h" //includes fastaFileReader, configFileReader, iostream and cmath
#include "SuffixTree/SuffixTree.h" //includes suffixTreeNode, STData, strings, time
#include "FileReader/alphabetfilereader.h"

using std::ofstream;

//application class to organize the execution of Project 3
class ReadMapApp {

public:
    //**********************************Constructors*************************************
    //TODO: Create constructor that takes necessary parameters from main
    ReadMapApp(const string & genomeFileName, const string & readFileName, const string & alphabetFileName);

    //*********************************Public Methods************************************
    //execution function, calls all private step functions in algorithm
    void run();



private:
    //*********************************Private Methods************************************
    
    //"Step functions:: Functions associated with steps in ReadMapping: MAIN algorithm (see course website)
    //TODO: modify parameters/return values as needed to execute in run method
    
    //Step 1: Construct Suffix Tree
    void constructST();
    //Step 2: Prepare Suffix Tree
    void prepareST();
    //Step 3:Map Reads
    void mapReads();
    //Step 3b: FindLoc
    int findLoc();
    //Step 3c: align
    //takes the length of the read, the index of
    void align(const vector<int> & indexList);
    //Step 4: output
    void output(int bestCoverage, int start, int end);

    //*********************************Private Data************************************
    ofstream outputFile;
    SuffixTree readTree;
    string gFileName, rFileName, aFileName;
    Sequence currentRead, G;
    int totalReads, alignedReads, constructionTime, prepTime, mapTime, outputTime, hitCount;
};


#endif