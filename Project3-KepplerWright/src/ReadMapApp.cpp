//Patrick  Keppler and Connor Wright, 2019

#include "ReadMapApp.h"

//**********************************Constructors*************************************

//TODO: modify this constructor as needed
ReadMapApp::ReadMapApp(const string & genomeFileName,const string & readFileName, const string & alphabetFileName)
{   
    gFileName = genomeFileName;
    rFileName = readFileName;
    aFileName = alphabetFileName;

    outputTime = alignedReads = totalReads = 0;

    outputFile.open("MappingResults_" + gFileName + ".txt");

    STData::startTimer();
    //call step 1
    constructST();
    STData::stopTimer();
    constructionTime = STData::getElapsedTime();
    //call step 2
    STData::startTimer();
    prepareST();
    STData::stopTimer();
    prepTime = STData::getElapsedTime();
}

//*********************************Public Methods************************************

//execution function, calls all private step functions in algorithm
void ReadMapApp::run()
{
    //call step 3
    STData::startTimer();
    mapReads();
    STData::stopTimer();
    mapTime = STData::getElapsedTime();
    outputFile.close();
    cout << "Genome Length: " << G.nucleotideSequence.length() << std::endl;
    cout << "Number of reads: " << totalReads << std::endl;
    cout << "Construction time: " << constructionTime << "ms";
    cout << ", Prep time: " << prepTime << "ms";
    cout << ", mapRead time: " << mapTime << "ms";
    cout << ", output time: " << outputTime / 1000 << "ms" << std::endl;
    cout << "Total time: " << constructionTime + prepTime + mapTime << "ms" << std::endl;
    cout << "Alignments per read: " << ((double) alignedReads)/totalReads;
}

//*********************************Private Methods************************************

//Step 1: Construct Suffix Tree
void ReadMapApp::constructST()
{
    fstream genomeReader(gFileName, fstream::in);
    fstream alphabetStream(aFileName, fstream::in);
    AlphabetFileReader alphabetReader(&alphabetStream);
    FastaFileReader genomeFastaReader(&genomeReader);
    G = genomeFastaReader.getNextSequence();
    G.nucleotideSequence += "$";
    readTree = SuffixTree();
    STData::init(&(G.nucleotideSequence), G.nucleotideSequence.length());
    readTree.McCreightInsert(&(G.nucleotideSequence)); 
}
//Step 2: Prepare Suffix Tree
void ReadMapApp::prepareST()
{
    readTree.DFS();
}
//Step 3:Map Reads
void ReadMapApp::mapReads()
{
    int readLoc = 0;
    fstream readsStream(rFileName, fstream::in);
    FastaFileReader readsReader(&readsStream);
    currentRead = readsReader.getNextSequence();
    while(currentRead.name != "" && currentRead.nucleotideSequence != "")
    {
        readLoc = findLoc();
        currentRead = readsReader.getNextSequence();
        totalReads++;
    }
}
//Step 3b: FindLoc
int ReadMapApp::findLoc()
{
    int readIndex = 0;
    vector<int> readLocs = readTree.findLocation(readIndex, &currentRead.nucleotideSequence);
    align(readLocs);

}
//Step 3c: align
void ReadMapApp::align(const vector<int> & indexList)
{
    int startPos, endPos, l, bestCoverage, j0, j1;
    int length = G.nucleotideSequence.length();
    AlignmentData aData;
    Sequence G_substring = {G.name, ""};

    bestCoverage = 0;

    //for each potential "best hit"
    for(const int & j : indexList) 
    {
        alignedReads++;
        //get the length of the read
        l = currentRead.nucleotideSequence.length();

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
        G_substring.nucleotideSequence = G.nucleotideSequence.substr(startPos, endPos - startPos);

        //perform alignment on the two strings
        Alignment readAlignment(currentRead, G_substring, nullptr, 1, -2, -5, -1);
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
    STData::startTimer2();
    output(bestCoverage, j0, j1);
    STData::stopTimer2();
    outputTime += STData::getElapsedTime2();

}
//Step 4: output
void ReadMapApp::output(int bestCoverage, int jStart, int jEnd)
{
    if(bestCoverage != 0)
    {
        outputFile << currentRead.name << " " << jStart << " " << jEnd << std::endl; 
    }
    else
    {
        outputFile << currentRead.name << " No hit found.\n";
    }
}
