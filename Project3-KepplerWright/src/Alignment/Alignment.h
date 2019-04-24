#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include "../FileReader/fastafilereader.h"
#include "../FileReader/configfilereader.h"
#include <iostream>
#include <cmath>
#include <limits>

using std::cin;
using std::cout;
using std::string;

typedef struct alignmentData {
  int max_score;
  unsigned int percentIdentity;
  unsigned int lengthCoverage;
  unsigned int bestHit_j0;
  unsigned int betsHit_j1;
}AlignmentData;


class Alignment {

public:
  Alignment(fstream * fastaFile = NULL, fstream * configFile = NULL);
  Alignment(Sequence & s1, Sequence & s2, fstream * configFile = NULL, int m_a = 1, int m_i = -2, int m_h = -5, int m_g = -1);
  ~Alignment();

  void doGlobalAlignment();
  void doLocalAlignment();

  void printOutput();
  void printReport();
  void printTable();

  AlignmentData getData();

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


  int mM, mN, mMatch, mMismatch,mH, mG, matchCount, mismatchCount, hCount, gCount, highestScore, highestScoreRow, highestScoreCol, total;
  ScoreCell ** dynamicTable;
  FastaFileReader mFastaReader;
  ConfigFileReader mConfigReader;
  Sequence s1, s2;
  bool isGlobal, tableReady;
  vector<AlignmentPair> printStack;
  AlignmentData aData;

  //helper functions for computing scores
  void computeScoreS(const int & i, const int & j);
  void computeScoreD( const int & i, const int & j);
  void computeScoreI(const int & i, const int & j);
  int myLog10(int n);
  int maxInt(int x, int y);

  //helper function for retracing a cell
  void retraceCell(int & i, int & j);


  //reset the table for doing different alignments
  void prepareTable();

};

#endif
