#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

void partOne() {
  string line;
  int twoLetterCount = 0;
  int threeLetterCount = 0;
  std::ifstream myfile("2.input");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      map<char, short> charCounts;
      for (char c : line) {
        if (charCounts.count(c)) {
          charCounts[c]++;
        } else {
          charCounts[c] = 1;
        }
      }
      bool twoMatch = false;
      bool threeMatch = false;

      for (char c : line) {
        twoMatch = charCounts[c] == 2 || twoMatch;
        threeMatch = charCounts[c] == 3 || threeMatch;
      }
      if (twoMatch) {
        twoLetterCount++;
      }
      if (threeMatch) {
        threeLetterCount++;
      }
    }
  }

  cout << "Two match count: " << to_string(twoLetterCount) << "\n";
  cout << "Three match count: " << to_string(threeLetterCount) << "\n";
  cout << "Checksum: " << to_string(twoLetterCount * threeLetterCount) << "\n";
}

void partTwo() {
  std::set<string> lines;
  std::ifstream myfile("2.input");
  if (myfile.is_open()) {
    string line;
    while (getline(myfile, line)) {
      lines.insert(line);
    }
  }

  for (auto line : lines) {
    for (auto other : lines) {
      if (other == line) {
        continue;
      }
      bool oneDiff = false;
      int lastDiff = -1;
      for (int i = 0; i < line.length(); i++) {
        if (line[i] == other[i]) {
          continue;
        }
        if (oneDiff) {
          oneDiff = false;
          break;
        }
        oneDiff = true;
        lastDiff = i;
      }
      if (oneDiff) {
        cout << "Close match found: " << line << " and " << other << "\n";
        line.erase(lastDiff, 1);
        cout << "Matching chars: " << line << "\n";
        return;
      }
    }
  }
}

int main() {
  cout << "==== Part One ====\n";
  partOne();
  cout << "\n==== Part Two ====\n";
  partTwo();
}
