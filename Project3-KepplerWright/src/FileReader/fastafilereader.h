#ifndef FASTAFILEREADER_H
#define FASTAFILEREADER_H
#include "filereader.h"
#include <vector>

using std::vector;

typedef struct sequence
{
  string name;
  string nucleotideSequence;
}Sequence;

class FastaFileReader: public FileReader {
public:
  FastaFileReader(fstream * infile = NULL);
  /*
  Sequence getSequenceByIndex(const int & index) const;
  Sequence getSequenceByName(const string & name) const;
  */
  Sequence getNextSequence();

private:
  //vector<Sequence> sequences;
  bool eof;
};

#endif
