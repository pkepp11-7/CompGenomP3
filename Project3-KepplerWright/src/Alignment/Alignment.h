#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "FileReader/fastafilereader.h"
#include "FileReader/configfilereader.h"
#include <iostream>
#include <cmath>

using std::cin;
using std::cout;
using std::string;

typedef struct alignmentData {
  unsigned int percentIdentity;
  unsigned int lengthCoverage;
}AlignmentData;


class Alignment {

public:
  Alignment(fstream * fastaFile = NULL, fstream * configFile = NULL);
  Alignment(Sequence s1, Sequence s2, int m_a = 1, int m_i = -2, int m_h = -5, int m_g = -1);

  void doGlobalAlignment();
  void doLocalAlignment();

  void printOutput();
  void printReport();
  void printTable();

private:
  typedef struct scorecell {
    int scoreS;
    int scoreD;
    int scoreI;
  }ScoreCell;

  typedef struct alignmentpair {
    char p1;
    char p2;
  }AlignmentPair;


  int mM, mN, mMatch, mMismatch,mH, mG, matchCount, mismatchCount, hCount, gCount, highestScore, highestScoreRow, highestScoreCol;
  ScoreCell ** dynamicTable;
  FastaFileReader mFastaReader;
  ConfigFileReader mConfigReader;
  Sequence s1, s2;
  bool isGlobal, tableReady;
  vector<AlignmentPair> printStack;

  //helper functions for computing scores
  void computeScoreS(const int & i, const int & j);
  void computeScoreD( const int & i, const int & j);
  void computeScoreI(const int & i, const int & j);
  int myLog10(int n);

  //helper function for retracing a cell
  void retraceCell(int & i, int & j);


  //reset the table for doing different alignments
  void prepareTable();

};

#endif
