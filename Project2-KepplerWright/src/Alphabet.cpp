//Patrick Keppler and Connor Wright, 2019
#include "Alphabet.h"

//default characters to none
unordered_map<char, unsigned short>Alphabet::charMap = unordered_map<char, unsigned short>();

//create a new alphabet from the string of
void Alphabet::createAlphabet(const string & chars)
{
  int length = chars.length();
  //empty the map of its previous alphabet.
  charMap.clear();
  charMap['$'] = 0;
  for(int i = 0; i < length; i++)
  {
    charMap[chars.at(i)] = (unsigned short)(i + 1);
  }
}

//compare individual characters
int Alphabet::compare(const char c1, const char c2)
{
  unsigned short index1, index2;
  //check that c1 and c2 are actually in the alphabet
  assert(charMap.find(c1) != charMap.end());
  assert(charMap.find(c2) != charMap.end());
  index1 = charMap[c1];
  index2 = charMap[c2];
  if(index1 < index2)
  {
    return -1;
  }
  if(index1 > index2)
  {
    return 1;
  }

  return 0;
}

//compare strings of characters
int Alphabet::compare(char * s1, char * s2)
{
  int i = 0, cmp;
  while(s1[i] != 0 && s2[i] != 0)
  {
    cmp = compare(s1[i], s2[i]);
    if(cmp != 0)
    {
      return cmp;
    }
  }
  if(s1[i] != 0)
  {
    return 1;
  }
  if(s2[i] != 0)
  {
    return -1;
  }
  return 0;

}
