#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

vector<pair<char, char>> readSequence() {
  string line;
  std::ifstream myfile("7.input");
  vector<pair<char, char>> lines;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      char req = line[5];
      char step = line[36];
      lines.push_back(make_pair(req, step));
    }
  }

  return lines;
}

void partOne(vector<pair<char, char>>* puzzle) {
  // When all elements of prerequisites[X] have finished, X can begin.
  map<char, set<char>> prerequisites;
  set<char> incomplete;

  for (auto step : *puzzle) {
    prerequisites[step.second].insert(step.first);

    // Assume everything is incomplete to begin with.
    // Use both elements to get a full list of nodes.
    incomplete.insert(step.first);
    incomplete.insert(step.second);
  }

  set<char> completed;

  // Anything with parents must be incomplete, so anything remaining has no
  // parents, and is therefore free to start.
  for (auto [step, parents] : prerequisites) {
    incomplete.erase(step);
  }

  // Print out the list of things that can start and remove them from the set of
  // incomplete tasks.
  for (char c : incomplete) {
    cout << c;
    completed.insert(c);
  }

  // While there are still tasks to process...
  while (!prerequisites.empty()) {
    set<char> availableThisIteration;
    // Check through all nodes that still remain.
    for (auto [step, parents] : prerequisites) {
      // Remove each completed node from the set of prerequisites.
      for (char c : completed) {
        parents.erase(c);
      }
      // If no prerequisites remain, add this to the set of tasks that can be
      // completed in this pass.
      if (parents.empty()) {
        availableThisIteration.insert(step);
      }
    }
    // Print out the set of available tasks, ordered alphabetically.
    for (char c : availableThisIteration) {
      completed.insert(c);
      // This task has been finished, no need to store it.
      prerequisites.erase(c);

      for (auto [step, parents] : prerequisites) {
        // Remove each completed node from the set of prerequisites.
        for (char c : completed) {
          parents.erase(c);
        }
        // If no prerequisites remain, add this to the set of tasks that can be
        // completed in this pass.
        if (parents.empty()) {
          availableThisIteration.insert(step);
        }
      }

      cout << c;
    }
  }
  cout << "\n";
}

void partTwo(vector<pair<char, char>>* puzzle) {}

int main() {
  auto puzzle = readSequence();
  cout << "==== Part One ====\n";
  partOne(&puzzle);
  cout << "\n==== Part Two ====\n";
  partTwo(&puzzle);
}
