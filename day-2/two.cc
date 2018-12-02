#include <fstream>
#include <iostream>
#include <map>
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
        charCounts[c]++;
      }
      bool twoMatch = false;
      bool threeMatch = false;

      for (auto [c, count] : line) {
        twoMatch = count == 2 || twoMatch;
        threeMatch = count == 3 || threeMatch;
      }
      if (twoMatch) {
        twoLetterCount++;
      }
      if (threeMatch) {
        threeLetterCount++;
      }
    }
  }

  cout << "Two match count: " << twoLetterCount << "\n";
  cout << "Three match count: " << threeLetterCount << "\n";
  cout << "Checksum: " << twoLetterCount * threeLetterCount << "\n";
}

void partTwo() {
  std::vector<string> lines;
  std::ifstream myfile("2.input");
  if (myfile.is_open()) {
    string line;
    while (getline(myfile, line)) {
      lines.push_back(line);
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
