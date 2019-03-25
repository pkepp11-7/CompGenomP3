//Patrick Keppler and Connor Wright, 2019
#include "STData.h"

//set default static variable values
unsigned int STData::internalNodes = 0, STData::leafNodes = 0, STData::len = 0, STData::position = 0;
char * STData::bwt = nullptr, * STData::inputStr = nullptr;


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

void STData::done()
{
  delete[] bwt;
  bwt = nullptr;
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

void STData::pushBwt(unsigned int index)
{
  if(index == 0)
  {
    index = len;
  }
  bwt[position] = inputStr[index - 1];
  position++;
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

void STData::printData()
{
  cout << "Internal nodes: " << internalNodes;
  cout << "\nLeaf nodes: " << leafNodes;
  cout << "\n";
}
