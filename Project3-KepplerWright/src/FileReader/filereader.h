#ifndef FILEREADER_H
#define FILEREADER_H
#include <fstream>
#include <string>

using std::fstream;
using std::string;

class FileReader {
private:
  fstream * myFile;

protected:
  //hidden function call so that subclasses can close the file after reading
  void doneReading();

public:
  FileReader(fstream * infile = NULL);
  virtual string readLine();
};

#endif
