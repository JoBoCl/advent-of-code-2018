#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

map<pair<int, int>, set<int>> readClaims() {
  string line;

  map<pair<int, int>, set<int>> claims;

  std::regex claimRegex("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");
  std::ifstream myfile("3.input");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      std::smatch results;
      auto match = std::regex_match(line, results, claimRegex);
      if (match) {
        try {
          int claim = stoi(results[1].str());
          int x = stoi(results[2].str());
          int y = stoi(results[3].str());
          int w = stoi(results[4].str());
          int h = stoi(results[5].str());

          for (int i = 0; i < w; i++) {
            for (int j = 0; j < h; j++) {
              auto l = make_pair(x + i, y + j);
              claims[l].insert(claim);
            }
          }
        } catch (invalid_argument ex) {
          cout << ex.what() << '\n';
          cout << results[0].str() << ", " << results[1].str() << ", "
               << results[2].str() << ", " << results[3].str() << ", "
               << results[4].str() << ", ";
        }
      }
    }
  }

  return claims;
}

void partOne(map<pair<int, int>, set<int>> claims) {
  int overlap = 0;
  for (auto [location, claimSet] : claims) {
    if (claimSet.size() >= 2) {
      overlap++;
    }
  }

  cout << "Overlapping claims: " << overlap << "\n";
}

void partTwo(map<pair<int, int>, set<int>> claims) {
  set<int> viable;
  set<int> nonviable;
  for (auto [location, claimSet] : claims) {
    if (claimSet.size() == 1) {
      viable.insert(claimSet.begin(), claimSet.end());
    } else {
      nonviable.insert(claimSet.begin(), claimSet.end());
    }
  }

  for (auto claim : nonviable) {
    viable.erase(claim);
  }

  cout << "Viable fabric claim: ";
  for (auto claim : viable) {
    cout << claim;
  }
  cout << "\n";
}

int main() {
  auto claims = readClaims();
  cout << "==== Part One ====\n";
  partOne(claims);
  cout << "\n==== Part Two ====\n";
  partTwo(claims);
}
