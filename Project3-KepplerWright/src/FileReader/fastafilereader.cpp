#include "fastafilereader.h"


FastaFileReader::FastaFileReader(fstream * infile) : FileReader(infile)
{
  eof = false;
  /*
  string currentName = "", currentSequence = "";
  sequences = vector<Sequence>();
  int index;
  string currentLine = readLine();
  while(currentLine.compare("end") != 0)
  {
    //header, contains a name
    if(currentLine.length() > 0 && currentLine[0] == '>')
    {
        index = 1;
        //check for the first whitespace character (space, tab, or null character)
        while(currentLine[index] != ' ' && currentLine[index] != 9 && currentLine[index] != 0)
        {
          index++;
        }
        currentName = currentLine.substr(1, index);
    }

    else if(currentLine.length() > 0)
    {
      //convert all characters to uppercase
      for(index = 0; index < currentLine.length(); index++)
      {
        if(currentLine[index] >= 'a' && currentLine[index] <= 'z')
        {
          currentLine[index] = currentLine[index] - 'a' + 'A';
        }
      }
      currentSequence += currentLine;
    }
    //empty line, add existing sequence to end of list if it exists
    else
    {
      if(currentName.length() > 0)
      {
        sequences.push_back(Sequence());
        sequences[sequences.size() - 1].name = currentName;
        sequences[sequences.size() - 1].nucleotideSequence = currentSequence;
        currentName = "";
        currentSequence = "";
      }
    }

    currentLine = readLine();
  }
  //if no empty line at end of the file, add the final sequence to the end of the list
  if(currentName.length() > 0)
  {
    sequences.push_back(Sequence());
    sequences[sequences.size() - 1].name = currentName;
    sequences[sequences.size() - 1].nucleotideSequence = currentSequence;
  }

  //call doneReading to close the file
  doneReading();
  */
}


/*
Sequence FastaFileReader::getSequenceByIndex(const int & index) const
{
  Sequence mySequence = {"", ""};
  //if valid index, return the struct at that index
  if(index < sequences.size())
  {
    mySequence = sequences[index];
  }
  //else return a struct with empty strings
  return mySequence;
}

Sequence FastaFileReader::getSequenceByName(const string & name) const
{
  int index = 0;
  Sequence mySequence = {"", ""};
  //search through the list of sequences for a matching name, return the first
  for(; index < sequences.size(); index++)
  {
    if(sequences[index].name.compare(name) == 0)
    {
      mySequence = sequences[index];
    }
  }
  //if not found, return a struct with empty strings
  return mySequence;
}
*/

//Access sequences in the order presented in the file.
//Will return an empty string in name and nucleotide_sequence when at end of file
Sequence FastaFileReader::getNextSequence() 
{
  int index;
  bool foundSequence = false;
  string currentName ="", currentSequence = "";
  string currentLine = readLine();
  Sequence sequence = {"", ""};

  while(!eof && !foundSequence && currentLine.compare("end") != 0)
  {
    //header, contains a name
    if(currentLine.length() > 0 && currentLine[0] == '>')
    {
        index = 1;
        //check for the first whitespace character (space, tab, or null character)
        while(currentLine[index] != ' ' && currentLine[index] != 9 && currentLine[index] != 0)
        {
          index++;
        }
        currentName = currentLine.substr(1, index);
    }

    else if(currentLine.length() > 0)
    {
      //convert all characters to uppercase
      for(index = 0; index < currentLine.length(); index++)
      {
        if(currentLine[index] >= 'a' && currentLine[index] <= 'z')
        {
          currentLine[index] = currentLine[index] - 'a' + 'A';
        }
      }
      currentSequence += currentLine;
    }
    //empty line, add existing sequence to end of list if it exists
    else
    {
      if(currentName.length() > 0)
      {

        sequence.name = currentName;
        sequence.nucleotideSequence = currentSequence;
        foundSequence = true;
      }
    }
    currentLine = readLine();
  }
  if(currentLine.compare("end") == 0)
  {
    eof = true;
    doneReading();
  }
  return sequence;
}