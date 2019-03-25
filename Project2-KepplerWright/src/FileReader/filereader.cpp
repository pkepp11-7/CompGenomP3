#include "filereader.h"

FileReader::FileReader(fstream * infile)
{
  myFile = infile;
}

string FileReader::readLine()
{
  string myString;
  if(myFile != NULL && myFile->is_open() && !myFile->eof())
  {
    std::getline(*myFile, myString);
    return myString;
  }
  return "end";
}

void FileReader::doneReading()
{
  if(myFile != NULL && myFile->is_open())
  {
    myFile->close();
  }
}
