#include "configfilereader.h"

ConfigFileReader::ConfigFileReader(fstream * infile) : FileReader(infile)
{

  matchRead = mismatchRead = hRead = gRead = false;
  //read each line of the file and parse it for each required value
  string lineString = readLine();
  while(lineString.compare("end") != 0)
  {
    parseLine(lineString);
    lineString = readLine();
  }
  //if a valid input is given for each line, file is valid
  valid = matchRead && mismatchRead && hRead && gRead;

  //call doneReading to close the file
  doneReading();
}

void ConfigFileReader::parseLine(const string & s)
{
  //line is for mismatch
  if(s.find("mismatch") != string::npos)
  {
    //"mismatch    (value)"
    mMismatch = atoi(&(s.c_str()[12]));
    mismatchRead = true;
  }
  //line is for match
  else if (s.find("match") != string::npos)
  {
    mMatch = atoi(&(s.c_str()[9]));
    matchRead = true;
  }
  //line is for h
  else if (s[0] == 'h')
  {
    mH = atoi(&(s.c_str()[5]));
    hRead = true;
  }
  //line is for g
  else if (s[0] == 'g')
  {
    mG =  atoi(&(s.c_str()[5]));
    gRead = true;
  }
}

//Will set the values of any read parameters. Only returns true if all parameters are read
bool ConfigFileReader::getConfigParameters(int & match, int & mismatch, int & h, int & g) const
{
  if(matchRead)
  {
      match = mMatch;
  }
  if(mismatchRead)
  {
      mismatch = mMismatch;
  }
  if(hRead)
  {
    h = mH;
  }
  if(gRead)
  {
      g = mG;
  }
  return valid;
}
