#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int main() {
  string line;
  std::vector<int> operations;
  std::map<int, int> firstSeenLine;
  std::ifstream myfile("1.input");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      operations.push_back(stoi(line));
    }
  }

  int value = 0;

  for (int op : operations) {
    value += op;
  }

  cout << "Final value: " + to_string(value) + "\n";

  value = 0;
  bool firstRepetition = false;
  int iterations = 0;

  while (!firstRepetition) {
    for (int op : operations) {
      value += op;
      iterations++;
      if (!firstRepetition && firstSeenLine.count(value)) {
        cout << "First saw " + to_string(value) + " at " +
                    to_string(firstSeenLine.find(value)->second) + "\n";
        firstRepetition = true;
      } else {
        firstSeenLine[value] = iterations;
      }
    }
  }
}
