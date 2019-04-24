#include "Alignment.h"

Alignment::Alignment(fstream * fastaFile, fstream * configFile)
{
  int index;
  //get necessary data from files
  mFastaReader = FastaFileReader(fastaFile);
  mConfigReader = ConfigFileReader(configFile);
  s1 = mFastaReader.getNextSequence();
  s2 = mFastaReader.getNextSequence();
  mM = s1.nucleotideSequence.length();
  mN = s2.nucleotideSequence.length();

  mConfigReader.getConfigParameters(mMatch, mMismatch, mH, mG);

  //dynamic programming table includes rows 1-M and columns 1-N with special row and clolumn 0 for base cases.
  dynamicTable = new ScoreCell * [mM + 1];
  for(index = 0; index <= mM; index++)
  {
    dynamicTable[index] = new ScoreCell[mN + 1];
  }
  prepareTable();
}

Alignment::Alignment(Sequence & seq1, Sequence & seq2, fstream * configFile, int m_a, int m_i, int m_h, int m_g)
{
  int index;
  //if config file supplied, get necessary data from files
  if(configFile)
  {
    mConfigReader = ConfigFileReader(configFile);
    mConfigReader.getConfigParameters(mMatch, mMismatch, mH, mG);
  }
  else {
    //get necessary data from parameters
    mMatch = m_a;
    mMismatch = m_i;
    mH = m_h;
    mG = m_g;
  }
  mM = seq1.nucleotideSequence.length();
  mN = seq2.nucleotideSequence.length();
  s1 = seq1;
  s2 = seq2;

  aData = {0, 0, 0, 0};

  //dynamic programming table includes rows 1-M and columns 1-N with special row and clolumn 0 for base cases.
  dynamicTable = new ScoreCell * [mM + 1];
  for(index = 0; index <= mM; index++)
  {
    dynamicTable[index] = new ScoreCell[mN + 1];
  }
  prepareTable();
}

//need to define a destructor to delete the dynamic programming table
Alignment::~Alignment()
{
  int i;
  for(i = 0; i <= mM; i++)
  {
    delete[] dynamicTable[i];
  }
  delete[] dynamicTable;

}

void Alignment::doGlobalAlignment()
{
  int i, j, highestScore;
  total = 0;
  matchCount = mismatchCount = hCount = gCount = 0;
  isGlobal = true;

  if(!tableReady)
  {
    prepareTable();
  }
  tableReady = false;

  //************************ Start Forward Computation *************************************
  for(i = 1; i <= mM; i++)
  {
    for(j = 1; j <= mN; j++)
    {
      computeScoreS(i, j);
      computeScoreD(i, j);
      computeScoreI(i, j);
    }
  }
 //*********************** End Forward Computation ***************************************

 //********************** Begin retrace **************************************************
  i = mM;
  j = mN;
  while(i > 0 || j > 0)
  {
    total++;
    retraceCell(i, j);
  }

  //******************* End retrace ****************************************************

}






void Alignment::doLocalAlignment()
{
  int i, j, cellMax;
  matchCount = mismatchCount = hCount = gCount = 0;
  isGlobal = false;

  total = 0;

  if(!tableReady)
  {
    prepareTable();
  }
  tableReady = false;

  highestScore = 0;
  //************************ Start Forward Computation *************************************

  for(i = 1; i <= mM; i++)
  {
    for(j = 1; j <= mN; j++)
    {
      computeScoreS(i, j);
      computeScoreD(i, j);
      computeScoreI(i, j);
      //keep track of the highest score as well as the location it was found to save time in retrace
      if(dynamicTable[i][j].scoreS > highestScore)
      {
        highestScore = dynamicTable[i][j].scoreS;
        highestScoreRow = i;
        highestScoreCol = j;
      }
      if(dynamicTable[i][j].scoreD > highestScore)
      {
        highestScore = dynamicTable[i][j].scoreD;
        highestScoreRow = i;
        highestScoreCol = j;
      }
      if(dynamicTable[i][j].scoreI > highestScore)
      {
        highestScore = dynamicTable[i][j].scoreI;
        highestScoreRow = i;
        highestScoreCol = j;
      }
    }
  }

  //*********************** End Forward Computation ***************************************

  //********************** Begin retrace **************************************************
  //start from the cell with the highest recorded score
  i = highestScoreRow;
  j = highestScoreCol;
  aData.max_score = highestScore;
  //set the end index of best hit to the character after the column of the optimal score (j1 is exclusive)
  aData.betsHit_j1 = j;
  cellMax = maxInt(maxInt(dynamicTable[i][j].scoreS, dynamicTable[i][j].scoreD), dynamicTable[i][j].scoreI);
  while(cellMax > 0)
  {
    retraceCell(i, j);
    total++;
    cellMax = maxInt(maxInt(dynamicTable[i][j].scoreS, dynamicTable[i][j].scoreD), dynamicTable[i][j].scoreI);
  }
  //set the start index of the best hit to the index of the end of the retrace
  aData.bestHit_j0 = j - 1;

}

int Alignment::myLog10(int n)
{
  int logN = 0;
  while(n / 10 > 0)
  {
    logN++;
    n /= 10;
  }
  return logN;
}


 int Alignment::maxInt(int x, int y)
 {
   if (x > y)
   {
     return x;
   }
   else return y;
 }

void Alignment::printOutput()
{
  int lineCount, s1Start, s2Start, s1End, s2End, index, spaces, spaces2, delta;
  lineCount = 0;
  s1Start = s2Start = 1;
  s1End = s2End = 0;
  AlignmentPair line[60];
  spaces = spaces2 = 0;
  delta = s2.name.length() - s1.name.length();

  cout << "Scores: match = " << mMatch << ", mismatch = " << mMismatch << ", h = " << mH << ", g = " << mG <<"\n\n";
  cout << "Sequence 1: \""  << s1.name << "\", length = " << mM << " characters\n";
  cout << "Sequence 2: \""  << s2.name << "\", length = " << mN << " characters\n";

  while(!printStack.empty())
  {
    if(lineCount == 60)
    {
      cout << s1.name;
      spaces = s2.name.length() - s1.name.length() + myLog10(s2Start) - myLog10(s1Start) + 2;
      if(spaces <= 0)
      {
        cout << "  ";
      }

      for(index = 0; index <  spaces; index++)
      {
        cout << ' ';
      }
      cout <<  s1Start << ' ';
      for(index = 0; index < 60; index++)
      {
        cout << line[index].p1;
      }
      cout << "  " << s1End << '\n';
      spaces = s1.name.length() + myLog10(s1Start) + 4;
      spaces2 = s2.name.length() + myLog10(s2Start) + 4;
      for(index = 0; (index < spaces) || (index < spaces2); index++)
      {
        cout << ' ';
      }
      for(index = 0; index < 60; index++)
      {
        if(line[index].p1 == line[index].p2)
        {
          cout << '|';
        }
        else
        {
          cout << ' ';
        }
      }
      cout << '\n';
      cout << s2.name;
      spaces = s1.name.length() - s2.name.length() + myLog10(s1Start) - myLog10(s2Start) + 2;
      if(spaces <= 0)
      {
        cout << "  ";
      }
      for(index = 0; index < spaces; index++)
      {
        cout << ' ';
      }
      cout << s2Start << ' ';
      for(index = 0; index < 60; index++)
      {
        cout << line[index].p2;
      }
      cout << "  " << s2End << "\n\n";
      lineCount = 0;
      s1Start = s1End + 1;
      s2Start = s2End + 1;
    }
    line[lineCount] = printStack.back();
    printStack.pop_back();

    if(line[lineCount].p1 != '-')
    {
      s1End++;
    }
    if(line[lineCount].p2 != '-')
    {
      s2End++;
    }
    lineCount++;
  }

  cout << s1.name;
  spaces = s2.name.length() - s1.name.length() + myLog10(s2Start) - myLog10(s1Start) +  2;
  if(spaces <= 0)
  {
    cout << "  ";
  }
  for(index = 0; index <  spaces; index++)
  {
    cout << ' ';
  }
  cout << s1Start << ' ';
  for(index = 0; index < lineCount; index++)
  {
    cout << line[index].p1;
  }
  cout << "  " << s1End << '\n';
  spaces = s1.name.length() + myLog10(s1Start) + 4;
  spaces2 = s2.name.length() + myLog10(s2Start) + 4;
  for(index = 0; (index < spaces) || (index < spaces2); index++)
  {
    cout << ' ';
  }
  for(index = 0; index < lineCount; index++)
  {
    if(line[index].p1 == line[index].p2)
    {
      cout << '|';
    }
    else
    {
      cout << ' ';
    }
  }
  cout << '\n';
  cout << s2.name;
  spaces = s1.name.length() - s2.name.length() + myLog10(s1Start) - myLog10(s2Start) + 2;
  if(spaces <= 0)
  {
    cout << "  ";
  }
  for(index = 0; index < spaces; index++)
  {
    cout << ' ';
  }
  cout << s2Start << ' ';
  for(index = 0; index < lineCount; index++)
  {
    cout << line[index].p2;
  }
  cout << "  " << s2End << "\n\n\n";

  printReport();

}

void Alignment::printReport()
{
  cout << "Report: \n";
  if(isGlobal)
  {
    cout << "Global Optimal Score = " << maxInt(maxInt(dynamicTable[mM][mN].scoreS, dynamicTable[mM][mN].scoreD), dynamicTable[mM][mN].scoreI) << '\n';
    cout << "Number of: matches = " << matchCount << ", mismatches = " << mismatchCount << ", gaps = " << gCount << ", opening gaps = " << hCount << '\n';
    int total = matchCount + mismatchCount + gCount;
    cout << "Identities = " << matchCount << '/' << total << " (" << matchCount * 100 / total << "%), Gaps = " << gCount << '/' << total << " (" << gCount * 100 / total << "%)\n";
  }
  else
  {
    cout << "Local Optimal Score = " << maxInt(maxInt(dynamicTable[highestScoreRow][highestScoreCol].scoreS, dynamicTable[highestScoreRow][highestScoreCol].scoreD), dynamicTable[highestScoreRow][highestScoreCol].scoreI) << '\n';
    cout << "Number of: matches = " << matchCount << ", mismatches = " << mismatchCount << ", gaps = " << gCount << ", opening gaps = " << hCount << '\n';
    int total = matchCount + mismatchCount + gCount;
    cout << "Identities = " << matchCount << '/' << total << " (" << matchCount * 100 / total << "%), Gaps = " << gCount << '/' << total << " (" << gCount * 100 / total << "%)\n";
  }
}

//get the data struct to return
AlignmentData Alignment::getData() 
{
  //Percent Identity: number of matches / alignment length
  aData.percentIdentity = matchCount * 100 / total;
  //Length Coverage: alignment length / l
  aData.lengthCoverage = total * 100 / mM;
  //start and end best hit index already set, ready to return.
  return aData;
}


void Alignment::computeScoreS(const int & i, const int & j)
{
  int prevS, prevD, prevI, newScore, addScore;
  prevS = dynamicTable[i-1][j-1].scoreS;
  prevD = dynamicTable[i-1][j-1].scoreD;
  prevI = dynamicTable[i-1][j-1].scoreI;

  if(s1.nucleotideSequence[i - 1] == s2.nucleotideSequence[j - 1])
  {
    addScore = mMatch;
  }
  else
  {
    addScore = mMismatch;
  }

  newScore = maxInt(maxInt(prevS, prevD), prevI);
  if(!isGlobal)
  {
    if (newScore + addScore > 0)
    {
      newScore += addScore;
    }
    else 
    {
      newScore = 0;
    }
  }
  else
  {
    if (newScore != std::numeric_limits<int>::min())
    {
      newScore += addScore;
    }
  }
  

  if(!isGlobal)
  {
    newScore = maxInt(newScore, 0);
  }

  dynamicTable[i][j].scoreS = newScore;

}


void Alignment::computeScoreD(const int & i, const int & j)
{
  int prevS, prevD, prevI, newScore;
  prevS = dynamicTable[i-1][j].scoreS;
  prevD = dynamicTable[i-1][j].scoreD;
  prevI = dynamicTable[i-1][j].scoreI;

  if(!isGlobal) {

    if(prevS > 0)
    {
      prevS += mH + mG;
    }
    else
    {
      prevS = 0;
    }
    if(prevD > 0)
    {
      prevD += mG;
    }
    else 
    {
      prevD = 0;
    }
    if(prevI  > 0 )
    {
      prevI += mH + mG;
    }
    else
    {
      prevI = 0;
    }
  }
  else {
    if(prevS != std::numeric_limits<int>::min())
    {
      prevS += mH + mG;
    }
    if(prevD != std::numeric_limits<int>::min())
    {
      prevD += mG;
    }
    if(prevI != std::numeric_limits<int>::min())
    {
      prevI += mH + mG;
    }
  }

  newScore = maxInt(maxInt(prevS, prevD), prevI);

  if(!isGlobal)
  {
    newScore = maxInt(newScore, 0);
  }

  dynamicTable[i][j].scoreD = newScore;

}


void Alignment::computeScoreI(const int & i, const int & j)
{
  int prevS, prevD, prevI, newScore;

  prevS = dynamicTable[i][j-1].scoreS;
  prevD = dynamicTable[i][j-1].scoreD;
  prevI = dynamicTable[i][j-1].scoreI;
  if(!isGlobal) 
  {
    if(prevS > 0)
    {
      prevS += mH + mG;
    }
    else
    {
      prevS = 0;
    }
    if(prevD > 0)
    {
      prevD += mH + mG;
    }
    else
    {
      prevD = 0;
    }
    if(prevI > 0)
    {
      prevI += mG;
    }
    else
    {
      prevI = 0;
    }
  }
  else 
  {
    if(prevS != std::numeric_limits<int>::min())
    {
      prevS += mH + mG;
    }
    if(prevI != std::numeric_limits<int>::min())
    {
      prevI += mG;
    }
    if(prevD != std::numeric_limits<int>::min())
    {
      prevD += mH + mG;
    }
  }
  newScore = maxInt(maxInt(prevS, prevD), prevI);

  if(!isGlobal)
  {
    newScore = (int)maxInt(newScore, 0);
  }

  dynamicTable[i][j].scoreI = newScore;

}


void Alignment::retraceCell(int & i, int & j)
{
  int curS, curD, curI, max;
  AlignmentPair pair = {0, 0};
  curS = dynamicTable[i][j].scoreS;
  curD = dynamicTable[i][j].scoreD;
  curI = dynamicTable[i][j].scoreI;

  max = maxInt(maxInt(curS, curD), curI);
  if(max == curS)
  {
    pair.p1 = s1.nucleotideSequence[i - 1];
    pair.p2 = s2.nucleotideSequence[j - 1];
    printStack.push_back(pair);
    if(pair.p1 == pair.p2)
    {
      matchCount++;
    }
    else
    {
      mismatchCount++;
    }
    i--;
    j--;
  }
  else if(max == curD)
  {
    pair.p1 = s1.nucleotideSequence[i-1];
    pair.p2 = '-';
    printStack.push_back(pair);
    gCount++;
    /*
    while(dynamicTable[i][j].scoreD - mG == dynamicTable[i-1][j].scoreD)
    {
        pair.p1 = s1.nucleotideSequence[i - 2];
        pair.p2 = '-';
        printStack.push_back(pair);
        gCount++;
        i--;
    }
    */
    if(curD - mG != dynamicTable[i-1][j].scoreD)
    {
      hCount++;
    }
    i--;
  }
  else {
    pair.p1 = '-';
    pair.p2 = s2.nucleotideSequence[j-1];
    printStack.push_back(pair);
    gCount++;
    /*
    while(dynamicTable[i][j].scoreI - mG == dynamicTable[i][j-1].scoreI)
    {
      pair.p1 = '-';
      pair.p2 = s2.nucleotideSequence[j-2];
      printStack.push_back(pair);
      gCount++;
      j--;
    }
    */
    if(curI - mG != dynamicTable[i][j-1].scoreI)
    {
      hCount++;
    }
    j--;
  }
}


void Alignment::prepareTable()
{
  int index;
  dynamicTable[0][0].scoreS = dynamicTable[0][0].scoreD = dynamicTable[0][0].scoreI = 0;
  if(isGlobal)
  {
    for(index = 1; index <= mM; index++)
    {
      dynamicTable[index][0].scoreD = mG * index + mH;
      dynamicTable[index][0].scoreS = dynamicTable[index][0].scoreI = std::numeric_limits<int>::min();
    }
    for(index = 1; index <= mN; index++)
    {
      dynamicTable[0][index].scoreI = mG * index + mH;
      dynamicTable[0][index].scoreS = dynamicTable[0][index].scoreD = std::numeric_limits<int>::min();
    }
  }
  else 
  {
    for(index = 1; index <= mM; index++)
    {
      dynamicTable[index][0].scoreD = dynamicTable[index][0].scoreS = dynamicTable[index][0].scoreI= 0;
    }
    for(index = 1; index <= mN; index++)
    {
      dynamicTable[0][index].scoreS = dynamicTable[0][index].scoreD = dynamicTable[0][index].scoreI = 0;;
    }
  }
  tableReady = true;
}


void Alignment::printTable()
{
  int i, j;

  cout << ",,";
  for(i = 0; i < s2.nucleotideSequence.length() - 1; i++)
  {
    cout << s2.nucleotideSequence[i] << ',';
  }
  cout <<  s2.nucleotideSequence[i] << "\n,";
  for(j = 0; j < mN; j++)
  {
    cout << dynamicTable[0][j].scoreS << ':' << dynamicTable[0][j].scoreD << ':' << dynamicTable[0][j].scoreI << ',';
  }
  cout << dynamicTable[0][j].scoreS << ':' << dynamicTable[0][j].scoreD << ':' << dynamicTable[0][j].scoreI << '\n';

  for(i = 1; i<= mM; i++)
  {
    cout << s1.nucleotideSequence[i - 1] << ',';
    for(j = 0; j < mN; j++)
    {
      cout << dynamicTable[i][j].scoreS << ':' << dynamicTable[i][j].scoreD << ':' << dynamicTable[i][i].scoreI << ',';
    }
    cout << dynamicTable[i][j].scoreS << ':' << dynamicTable[i][j].scoreD << ':' << dynamicTable[i][j].scoreI << '\n';
  }

}
