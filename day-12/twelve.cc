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

std::pair<std::set<int>, std::set<char>> readSequence() {
  std::ifstream myfile("12.input");
  std::set<int> initialState;
  std::set<char> transitions;
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
      if (line[9] == '#') {
        transitions.insert(p);
      }
    }
  }

  return std::make_pair(initialState, transitions);
}

void iterate(std::pair<std::set<int>, std::set<char>> *puzzle,
             long iterations) {
  std::set<int> state = puzzle->first;
  std::set<int> newState;

  for (auto t : puzzle->second) {
    // std::cout << (int)t << ", ";
  }
  // std::cout << "\n";

  for (long n = 0; n < iterations; n++) {
    newState.clear();
    std::cout << (n < 10 ? " " : "") << n << ": ";
    int startVal = *state.begin() - 2;
    int endVal = *state.rbegin() + 2;

    for (int i = startVal; i <= endVal; i++) {
      std::cout << (state.count(i) ? '#' : '.');
      char p = 0;
      for (int j = 0; j < 5; j++) {
        p |= state.count(i + j - 2) << (4 - j);
      }
      if (puzzle->second.count(p) == 1) {
        newState.insert(i);
      }
    }

    std::cout << " (" << startVal << ", " << endVal << ")\n";
    state = newState;
  }

  int sum = 0;

  for (auto pos : state) {
    sum += pos;
  }

  std::cout << "Total value in pots: " << sum;
}

void partOne(std::pair<std::set<int>, std::set<char>> *puzzle) {
  iterate(puzzle, 20);
}
void partTwo(std::pair<std::set<int>, std::set<char>> *puzzle) {
  iterate(puzzle, 200);
  // iterate(puzzle, 50000000000);
  // This would have taken approx 5.83 days to calculate.
  //
  // Fortunately, the columns eventually reached a steady-state, consistently
  // moving one left.  After analysing this pattern, it is possible to
  // calculate the end positions by hand, and then get a sum.
}

int main() {
  auto puzzle = readSequence();
  std::cout << "==== Part One ====\n";
  partOne(&puzzle);
  std::cout << "\n==== Part Two ====\n";
  partTwo(&puzzle);
}
