#ifndef CONFIGFILEREADER_H
#define CONFIGFILEREADER_H

#include "filereader.h"
#include <cstdlib>
class ConfigFileReader: public FileReader {
public:
  ConfigFileReader(fstream * infile = NULL);
  bool getConfigParameters(int & match, int & mismatch, int & h, int & g) const;
private:
  bool valid, matchRead, mismatchRead, hRead, gRead;
  int mMatch, mMismatch, mH, mG;
  void parseLine(const string & s);
};

#endif
