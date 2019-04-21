//Patrick Keppler and Connor Wright, 2019
#ifndef READMAPAPP_H
#define READMAPAPP_H

#include "Alignment/Alignment.h" //includes fastaFileReader, configFileReader, iostream and cmath
#include "SuffixTree/SuffixTree.h" //includes suffixTreeNode, STData, strings, time
#include "FileReader/alphabetfilereader.h"

//application class to organize the execution of Project 3
class ReadMapApp {

public:
    //**********************************Constructors*************************************
    //TODO: Create constructor that takes necessary parameters from main
    ReadMapApp();

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
    void findLoc();
    //Step 3c: align
    void align();
    //Step 4: output
    void output();

    //*********************************Private Data************************************
};


#endif