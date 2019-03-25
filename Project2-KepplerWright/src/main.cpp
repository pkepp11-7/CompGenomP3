//Patrick Keppler and Connor Wright, 2019
#include "FileReader/fastafilereader.h"
#include "SuffixTree/SuffixTree.h"

int main(int argc, char * argv[])
{
  int testMode, length;
  char * cString;
  fstream fastaFile;
  SuffixTree suffixTree;
  FastaFileReader fastaReader;
  string fastaString;
  assert(argc >= 3);

  fastaFile.open(argv[1], fstream::in);
  testMode = 0;

  if(argc > 3)
  {
    testMode = atoi(argv[4]);
  }

  if(fastaFile.is_open())
  {
      fastaReader = FastaFileReader(&fastaFile);
      fastaString = fastaReader.getSequenceByIndex(0).name;
      //add the $ to the end of the input string
      fastaString.append("$");
      //get char *, length
      length = fastaString.length();
      const char * immutableStr = fastaString.c_str();
      cString = new char[length];
      strcpy(cString, immutableStr);
      //create a suffix tree
      suffixTree = SuffixTree();
      //TODO:replace with McCreight when ready
      suffixTree.basicInsert(cString, length);
      //1's place is 1: do dfs
      if(testMode & 1 == 1)
      {
        STData::init(cString, length);
        suffixTree.DFS();
        //2's place is 1: print bwt
        if(testMode & 2 == 2)
        {
          STData::printBwt();
        }
        //4's place is 1: print other data
        if(testMode & 4 == 4)
        {
          STData::printData();
        }
        //done, delete BWT
        STData::done();
      }

  }


  return 0;
}
