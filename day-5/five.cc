#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

string readSequence() {
  string line;
  std::ifstream myfile("5.input");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      return line;
    }
  }
  return "";
}

string reduce(string sequence) {
  string newSequence = sequence;
  string oldSequence = sequence;

  do {
    oldSequence = newSequence;
    for (int i = 0; i < newSequence.length(); i++) {
      while (toupper(newSequence[i]) == toupper(newSequence[i + 1]) &&
             newSequence[i] != newSequence[i + 1]) {
        newSequence = newSequence.erase(i, 2);
      }
    }
  } while (newSequence != oldSequence);

  return newSequence;
}

void partOneSlow(string sequence) {
  string regexString = "(";

  for (char i = 0; i < 26; i++) {
    string upper(1, (char)65 + i);
    string lower(1, (char)97 + i);
    regexString.append(upper);
    regexString.append(lower);
    regexString.append("|");
    regexString.append(lower);
    regexString.append(upper);
    regexString.append("|");
  }

  regexString.append(")");

  regex pairs(regexString);
  string oldString = sequence;
  string newString = sequence;
  do {
    oldString = newString;
    newString = regex_replace(oldString, pairs, "");
  } while (newString != oldString);

  cout << "Sequence length: " << oldString.length();
}

void partTwo(string sequence) {
  int shortest = -1;
  int length = 500001;
  for (char i = 0; i < 26; i++) {
    char upper = 65 + i;
    char lower = 97 + i;
    string newSequence = sequence;
    for (int i = 0; i < newSequence.length(); i++) {
      while (newSequence[i] == upper || newSequence[i] == lower) {
        newSequence = newSequence.erase(i, 1);
      }
    }
    int current = reduce(newSequence).length();
    if (current < length) {
      shortest = i;
      length = current;
    }
  }

  cout << "Sequence length: " << length << "\n";
}

void partOne(string sequence) {
  cout << "Sequence length: " << reduce(sequence).length();
}

int main() {
  string sequence = readSequence();
  cout << "==== Part One ====\n";
  partOne(sequence);
  cout << "\n==== Part Two ====\n";
  partTwo(sequence);
}
