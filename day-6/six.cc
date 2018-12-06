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

struct Puzzle {
  vector<pair<int, int>> points;
  pair<int, int> bounds;
};

Puzzle readSequence() {
  string line;
  std::ifstream myfile("6.input");
  vector<pair<int, int>> lines;
  int maxX = -1;
  int maxY = -1;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      size_t ptr = 0;
      int first = stoi(line, &ptr);
      int second = stoi(line.substr(ptr + 1));
      if (first > maxX) {
        maxX = first;
      }
      if (second > maxY) {
        maxY = second;
      }
      lines.push_back(make_pair(first, second));
    }
  }
  struct Puzzle puzzle;
  puzzle.bounds = make_pair(maxX + 1, maxY + 1);
  puzzle.points = lines;
  return puzzle;
}

int manhattan(pair<int, int> a, pair<int, int> b) {
  return abs(a.first - b.first) + abs(a.second - b.second);
}

int minId(pair<int, int> a, vector<pair<int, int>> positions) {
  map<int, int> pointsByDistance;
  int id = 1;
  for (pair<int, int> p : positions) {
    int md = manhattan(a, p);
    if (pointsByDistance[md] != 0) {
      pointsByDistance[md] = -1;
    } else {
      pointsByDistance[md] = id;
    }
    id++;
  }
  return pointsByDistance.begin()->second;
}

void partOne(Puzzle* puzzle) {
  pair<int, int> maxSize = puzzle->bounds;
  const int maxX = maxSize.first;
  const int maxY = maxSize.second;

  int** array = new int*[maxX];

  // id to size
  map<int, int> sizes;

  for (int x = 0; x < maxX; x++) {
    array[x] = new int[maxY];
    for (int y = 0; y < maxY; y++) {
      int id = minId(make_pair(x, y), puzzle->points);
      if (x == 0 || x + 1 == maxX || y == 0 || y + 1 == maxY) {
        sizes[id] = numeric_limits<int>::min();
      } else if (id != -1 && sizes[id] != numeric_limits<int>::min()) {
        sizes[id]++;
      }
    }
  }

  int biggestId = -1;
  int biggestSize = 0;
  for (auto [id, size] : sizes) {
    if (size > biggestSize) {
      biggestId = id;
      biggestSize = size;
    }
  }

  cout << "Biggest area: " << biggestSize;
}

int totalDistance(pair<int, int> p, vector<pair<int, int>> locs) {
  vector<int> distances;
  std::transform(locs.begin(), locs.end(), back_inserter(distances),
                 [&p](pair<int, int> l) -> int { return manhattan(l, p); });
  return std::accumulate(distances.begin(), distances.end(), 0);
}

void partTwo(Puzzle* puzzle) {
  pair<int, int> maxSize = puzzle->bounds;
  const int maxX = maxSize.first;
  const int maxY = maxSize.second;

  int size = 0;

  for (int x = 0; x < maxX; x++) {
    for (int y = 0; y < maxY; y++) {
      if (totalDistance(make_pair(x, y), puzzle->points) < 10000) {
        size++;
      }
    }
  }

  cout << "Total size of area: " << size << "\n";
}

int main() {
  auto puzzle = readSequence();
  cout << "==== Part One ====\n";
  partOne(&puzzle);
  cout << "\n==== Part Two ====\n";
  partTwo(&puzzle);
}
