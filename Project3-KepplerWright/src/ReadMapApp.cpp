//Patrick Keppler and Connor Wright, 2019

#include "ReadMapApp.h"

//**********************************Constructors*************************************

//TODO: modify this constructor as needed
ReadMapApp::ReadMapApp()
{

}

//*********************************Public Methods************************************

//execution function, calls all private step functions in algorithm
void ReadMapApp::run()
{
    //call step 1
    constructST();
    //call step 2
    prepareST();
    //call step 3
    mapReads();
    //call output
    output();
}

//*********************************Private Methods************************************

//Step 1: Construct Suffix Tree
void ReadMapApp::constructST()
{

}
//Step 2: Prepare Suffix Tree
void ReadMapApp::prepareST()
{

}
//Step 3:Map Reads
void ReadMapApp::mapReads()
{
    
}
//Step 3b: FindLoc
void ReadMapApp::findLoc()
{

}
//Step 3c: align
void ReadMapApp::align(const vector<unsigned int> & indexList, Sequence * G, Sequence * r_i)
{
    unsigned int startPos, endPos, l, bestCoverage, j0, j1;
    int length = indexList.size();
    Alignment readAlignment;
    AlignmentData aData;
    Sequence G_substring = {G->name, ""};

    bestCoverage = 0;

    //for each potential "best hit"
    for(const unsigned int & j : indexList) 
    {
        //get the length of the read
        l = r_i->nucleotideSequence.length();

        //check for edge cases for values of j
        if(j < l)
        {
            //if j - l < 0, set to start at 0
            startPos = 0;
        }
        else
        {
            //else start at position j - l
            startPos = j - l;
        }
        if(j + l > length)
        {
            //if j + l exceeds the end of G, end at the end of G
            endPos = length;
        }
        else
        {
            //else end at j + l
            endPos = j + l;
        }
        //Create a Sequence with a substring of G, based on previous startPos and endPos
        G_substring.nucleotideSequence = G->nucleotideSequence.substr(startPos, endPos - startPos);

        //perform alignment on the two strings
        readAlignment = Alignment(*r_i, G_substring);
        readAlignment.doLocalAlignment();
        aData = readAlignment.getData();

        //if the percent identity is > 90% and the length coverage > 80%
        if(aData.percentIdentity > 90 && aData.lengthCoverage > 80) 
        {
            if(aData.lengthCoverage > bestCoverage)
            {
                //if this is the best coverage encountered for any j in Li, save it
                j0 = aData.bestHit_j0 + startPos;
                j1 = aData.betsHit_j1 + startPos;
                bestCoverage = aData.lengthCoverage;
            }
        }
    }
}
//Step 4: output
void ReadMapApp::output()
{

}
