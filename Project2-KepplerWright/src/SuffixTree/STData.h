//Patrick Keppler and Connor Wright, 2019
#ifndef STDATA_H
#define STDATA_H

#include <iostream>

using std::cout;

//Static class for handling data from the suffix tree. Call "init: before using.
class STData {
private:
  static unsigned int internalNodes, leafNodes, len, position;
  static char * bwt, * inputStr;

public:

  //increment counters for notable values
  static void incrementInternalNodes();
  static void incrementLeafNodes();
  static void pushBwt(unsigned int index);

  //initialize the static class
  static void init(char * str, const unsigned int & length);
  //destroy the dynamically-sized bwt table
  static void done();

  //print the bwt 1 character to a line
  static void printBwt();
  //print non-bwt data
  static void printData();

};

#endif
