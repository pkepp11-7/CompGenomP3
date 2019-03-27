//Patrick Keppler and Connor Wright, 2019
#include "STData.h"

//set default static variable values
unsigned int STData::internalNodes = 0, STData::leafNodes = 0, STData::len = 0, STData::position = 0;
char * STData::bwt = nullptr, * STData::inputStr = nullptr;
struct timeval * STData::startTime = nullptr, * STData::endTime = nullptr;
SuffixTreeNode * STData::deepestInternal = nullptr;


//******private methods***********
string STData::constructLongestRepeat(SuffixTreeNode * currentNode)
{
  if(currentNode->getId() == 0)
  {
    return "";
  }
  else return constructLongestRepeat(currentNode->getParentPtr()) + currentNode->getLabel();
}

//*******public methods*************

//initialize the static class
void STData::init(char * str, const unsigned int & length)
{
  len = length;
  inputStr =  str;
  position = 0;
  internalNodes = 0;
  leafNodes = 0;

  if(bwt != nullptr)
  {
    done();
  }
  bwt = new char[length];
}

//destroy the dynamically-sized bwt table, as well as timers if used
void STData::done()
{
  if(bwt != nullptr)
  {
    delete[] bwt;
    bwt = nullptr;
  }
  if(startTime != nullptr)
   {
    delete startTime;
  }
  if(endTime != nullptr)
  {
    delete endTime;
  }

  startTime = endTime = nullptr;
}


//increments the internal node counter
void STData::incrementInternalNodes()
{
  internalNodes++;
}

//increment the leaf node counter
void STData::incrementLeafNodes()
{
  leafNodes++;
}

void STData::findLongestRepeat(SuffixTreeNode * inNode)
{
  //if no deepest internal node || deepest internal node is shallower than input node
  if(deepestInternal == nullptr || deepestInternal->getDepth() < inNode->getDepth())
  {
    deepestInternal = inNode;
  }
}


void STData::pushBwt(unsigned int index)
{
  if(index == 0)
  {
    index = len;
  }
  bwt[position] = inputStr[index - 1];
  position++;
}

void STData::startTimer()
{
  if(startTime == nullptr)
  {
    startTime = new struct timeval();
  }
  gettimeofday(startTime, NULL);
}

void STData::stopTimer()
{
  if(stopTime == nullptr)
  {
    stopTime = new struct timeval();
  }
  gettimeofday(stopTime, NULL);
}

//print the bwt 1 character to a line
void STData::printBwt()
{
  int i;
  for(i = 0; i < position; i++)
  {
    cout << bwt[i] << '\n';
  }
}

//print non-bwt data
void STData::printData()
{
  cout << "Internal nodes: " << internalNodes;
  cout << "\nLeaf nodes: " << leafNodes;
  cout << "\n";
}

//print time elapsed between startTimer and stopTimer calls
void STData::printElapsedTime()
{
  //elapsed time in milliseconds
  assert(startTime != nullptr && stopTime != nullptr);
  unsigned int elapsedTimeMs = (stopTime->tv_sec - startTime->tv_sec) * 1000 + (stopTime->tv_usec - stopTime->tv_usec) / 1000;
  cout << "ST construction time: " << elapsedTimeMs << "ms\n";

}

//print the longest repeating substring, based on the deepest internal node
void STData::printLongestRepeat()
{
  string longestRepeat;
  if(deepestInternal != nullptr)
  {
    longestRepeat = constructLongestRepeat(deepestInternal);
  }
  cout << "Longest Repeating segment: " << longestRepeat << '\n';
}
