//Patrick Keppler and Connor Wright, 2019
#ifndef ALPHABET_H
#define ALPHABET_H

#include <string>
#include <unordered_map>
#include <cassert>

using std::string;
using std::unordered_map;

class Alphabet {
private:
  static unordered_map<char, unsigned short> charMap;
public:
  static void createAlphabet(const string & chars);
  static int compare(const char c1, const char c2);
  static int compare(char * s1, char * s2);
};

#endif
