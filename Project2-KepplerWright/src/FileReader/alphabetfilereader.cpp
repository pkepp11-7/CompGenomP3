//Patrick Keppler and Connor Wright, 2019
#include "alphabetfilereader.h"

AlphabetFileReader::AlphabetFileReader(fstream * infile) : FileReader(infile)
{
  string rawAlphabet = readLine();
  string smallAlphabet;
  int i, length = rawAlphabet.length();
  for(i = 0; i < length; i+=2)
  {
    smallAlphabet.push_back(rawAlphabet.at(i));
  }
  Alphabet::createAlphabet(smallAlphabet);
}
