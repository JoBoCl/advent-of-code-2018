#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
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

void partOne(vector<pair<char, char>> *puzzle) {
  // When all elements of prerequisites[X] have finished, X can begin.
  map<char, set<char>> prerequisites;
  set<char> startNodes;

  for (auto step : *puzzle) {
    prerequisites[step.second].insert(step.first);

    // Assume everything is startNodes to begin with.
    // Use both elements to get a full list of nodes.
    startNodes.insert(step.first);
    startNodes.insert(step.second);
  }

  set<char> completed;
  set<char> visited;

  // Anything with parents must be startNodes, so anything remaining has no
  // parents, and is therefore free to start.
  for (auto [step, parents] : prerequisites) {
    startNodes.erase(step);
  }

  for (auto c : startNodes) {
    set<char> empty;
    prerequisites[c] = empty;
  }

  // While there are still tasks to process...
  while (!prerequisites.empty()) {
    char availableThisIteration;
    // Check through all nodes that still remain.
    for (auto [step, parents] : prerequisites) {
      // Remove each completed node from the set of prerequisites.
      for (char c : completed) {
        parents.erase(c);
      }
      // If no prerequisites remain, add this to the set of tasks that can be
      // completed in this pass.
      if (parents.empty()) {
        availableThisIteration = step;
        break;
      }
    }
    // Print out the set of available tasks, ordered alphabetically.
    char c = availableThisIteration;
    completed.insert(c);
    // This task has been finished, no need to store it.
    prerequisites.erase(c);
    cout << c;
  }

  cout << "\n";
}

const char totalWork(const char c) { return c - 'A' + 61; }

pair<char, int> nextAvailable(map<char, int> *workerPool) {
  int time = numeric_limits<int>::max();
  char worker;
  for (auto [w, t] : *workerPool) {
    if (t < time) {
      worker = w;
      time = t;
    }
  }
  return make_pair(worker, time);
}

void partTwo(vector<pair<char, char>> *puzzle) {
  // When all elements of prerequisites[X] have finished, X can begin.
  map<char, set<char>> prerequisites;
  set<char> startNodes;
  map<char, int> workerPool;

  for (int i = 0; i < 5; i++) {
    workerPool[i] = 0;
  }

  for (auto step : *puzzle) {
    prerequisites[step.second].insert(step.first);

    // Assume everything is startNodes to begin with.
    // Use both elements to get a full list of nodes.
    startNodes.insert(step.first);
    startNodes.insert(step.second);
  }

  map<char, int> completed;
  set<char> visited;

  // Anything with parents must be startNodes, so anything remaining has no
  // parents, and is therefore free to start.
  for (auto [step, parents] : prerequisites) {
    startNodes.erase(step);
  }

  for (auto c : startNodes) {
    set<char> empty;
    prerequisites[c] = empty;
  }

  int time = 0;

  // While there are still tasks to process...
  while (!prerequisites.empty()) {
    set<char> availableThisIteration;
    // Check through all nodes that still remain.
    for (auto [step, parents] : prerequisites) {
      // Remove each completed node from the set of prerequisites.
      for (auto [c, t] : completed) {
        if (time >= t) {
          parents.erase(c);
        }
      }
      // If no prerequisites remain, add this to the set of tasks that can be
      // completed in this pass.
      if (parents.empty()) {
        availableThisIteration.insert(step);
      }
    }
    for (char c : availableThisIteration) {
      auto nextAvailableWorker = nextAvailable(&workerPool);
      if (nextAvailableWorker.second <= time) {
        workerPool[nextAvailableWorker.first] = time + totalWork(c);
        completed[c] = time + totalWork(c);
        cout << "Time " << time << " worker #"
             << (char)(49 + nextAvailableWorker.first) << " starts work on "
             << c << " for " << (int)totalWork(c) << "s.\n";
        // This task has been finished, no need to store it.
        prerequisites.erase(c);
      }
    }
    time++;
  }

  int maxTime = 0;
  for (auto [c, t] : workerPool) {
    if (t > maxTime) {
      maxTime = t;
    }
  }

  cout << "Total time: " << maxTime;
  cout << "\n";
}

int main() {
  auto puzzle = readSequence();
  cout << "==== Part One ====\n";
  partOne(&puzzle);
  cout << "\n==== Part Two ====\n";
  partTwo(&puzzle);
}
