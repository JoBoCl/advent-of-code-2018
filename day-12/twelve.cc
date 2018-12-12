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

std::pair<std::map<int, bool>, std::map<char, bool>> readSequence() {
  std::ifstream myfile("test.input");
  std::map<int, bool> initialState;
  std::map<char, bool> transitions;
  std::string line;
  if (myfile.is_open()) {
    if (getline(myfile, line)) {
      for (int i = 15; i < line.length(); i++) {
        initialState[i - 15] = (line[i] == '#');
      }
    }
    getline(myfile, line); // Skip empty line.
    while (getline(myfile, line)) {
      char p = 0;
      for (int i = 0; i < 5; i++) {
        p += line[i] == '#' ? (char)1 << (4 - i) : 0;
      }
      transitions[p] = line[9] == '#';
    }
  }

  return std::make_pair(initialState, transitions);
}

void partOne(std::pair<std::map<int, bool>, std::map<char, bool>> *puzzle) {
  std::map<int, bool> state = puzzle->first;
  std::map<int, bool> newState;
  newState.insert(state.begin(), state.end());

  int startVal = 0;
  int endVal = state.size();

  for (int n = 0; n <= 20; n++) {
    std::cout << (n < 10 ? " " : "") << n << ": ";

    for (int i = startVal; i < endVal; i++) {
      if (-3 <= i && i <= 38) {
        std::cout << (state[i] ? '#' : '.');
      }
      char p = 0;
      for (int j = -2; j < 3; j++) {
        p += state[i + j] ? (char)1 << (2 - j) : 0;
      }
      newState[i] = (puzzle->second[p]);
    }

    state.clear();
    state.insert(newState.begin(), newState.end());
    newState.clear();
    if (state.begin()->second) {
      startVal -= 4;
    }
    if (state.rbegin()->second) {
      endVal += 4;
    }
    std::cout << " (" << startVal << ", " << endVal << ")\n";
  }
  int sum = 0;

  for (auto [pos, plant] : state) {
    if (plant) {
      sum += pos;
    }
  }

  std::cout << "Total value in pots: " << sum;
}

void partTwo(std::pair<std::map<int, bool>, std::map<char, bool>> *puzzle) {}

int main() {
  auto puzzle = readSequence();
  std::cout << "==== Part One ====\n";
  partOne(&puzzle);
  std::cout << "\n==== Part Two ====\n";
  partTwo(&puzzle);
}
