#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

std::pair<std::set<int>, std::map<char, bool>> readSequence() {
  std::ifstream myfile("test.input");
  std::set<int> initialState;
  std::map<char, bool> transitions;
  std::string line;
  if (myfile.is_open()) {
    if (getline(myfile, line)) {
      for (int i = 15; i < line.length(); i++) {
        if (line[i] == '#') {
          initialState.insert(i - 15);
        }
      }
    }
    getline(myfile, line); // Skip empty line.
    while (getline(myfile, line)) {
      char p = 0;
      for (int i = 0; i < 5; i++) {
        p |= (line[i] == '#') << (4 - i);
      }
      transitions[p] = line[9] == '#';
    }
  }

  return std::make_pair(initialState, transitions);
}

void partOne(std::pair<std::set<int>, std::map<char, bool>> *puzzle) {
  std::set<int> state = puzzle->first;
  std::set<int> newState;

  int startVal = *state.begin() - 2;
  int endVal = *state.rbegin() + 2;

  for (int n = 0; n <= 20; n++) {
    newState.clear();
    std::cout << (n < 10 ? " " : "") << n << ": ";

    for (int i = startVal; i <= endVal; i++) {
      if (-3 <= i && i <= 38) {
        std::cout << (state.count(i) ? '#' : '.');
      }
      char p = 0;
      for (int j = 0; j < 5; j++) {
        p |= state.count(i + j - 2) << (4 - j);
      }
      if (puzzle->second[p]) {
        newState.insert(i);
      }
    }

    std::cout << " (" << startVal << ", " << endVal << ")\n";
    state = newState;
    startVal = *state.begin() - 2;
    endVal = *state.rbegin() + 2;
  }

  int sum = 0;

  for (auto pos : state) {
    sum += pos;
  }

  std::cout << "Total value in pots: " << sum;
}

void partTwo(std::pair<std::set<int>, std::map<char, bool>> *puzzle) {}

int main() {
  auto puzzle = readSequence();
  std::cout << "==== Part One ====\n";
  partOne(&puzzle);
  std::cout << "\n==== Part Two ====\n";
  partTwo(&puzzle);
}
