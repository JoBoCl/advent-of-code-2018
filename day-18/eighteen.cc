#include <algorithm>
#include <cassert>
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

enum Acre { EMPTY, WOOD, LUMBER };

std::vector<std::vector<Acre>> readSequence() {
  std::ifstream myfile("18.input");
  std::vector<std::vector<Acre>> puzzle;
  std::string line;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      std::vector<Acre> row;
      for (char c : line) {
        switch (c) {
          case '.':
            row.push_back(EMPTY);
            break;
          case '|':
            row.push_back(WOOD);
            break;
          case '#':
            row.push_back(LUMBER);
            break;
        }
      }
      puzzle.push_back(row);
    }
  }

  return puzzle;
}

int adjacent(std::vector<std::vector<Acre>> *area, int x, int y, Acre type) {
  int width = (*area)[0].size();
  int height = area->size();
  int count = 0;

  for (int i = -1; i < 2; i++) {
    if (0 <= y + i && y + i < height) {
      for (int j = -1; j < 2; j++) {
        if (0 <= x + j && x + j < width) {
          if ((*area)[y + i][x + j] == type) {
            if (i != 0 || j != 0) {
              count++;
            }
          }
        }
      }
    }
  }

  assert(0 <= count && count <= 8);
  return count;
}

int step(std::vector<std::vector<Acre>> &area) {
  int width = (area)[0].size();
  int height = area.size();

  std::vector<std::vector<Acre>> next;

  for (int i = 0; i < height; i++) {
    std::vector<Acre> row;
    for (int j = 0; j < width; j++) {
      int count, wood, lumber;
      switch ((area)[i][j]) {
        case EMPTY:
          count = adjacent(&area, j, i, WOOD);
          if (count >= 3) {
            row.push_back(WOOD);
          } else {
            row.push_back(EMPTY);
          }
          break;
        case WOOD:
          count = adjacent(&area, j, i, LUMBER);
          if (count >= 3) {
            row.push_back(LUMBER);
          } else {
            row.push_back(WOOD);
          }
          break;
        case LUMBER:
          wood = adjacent(&area, j, i, WOOD);
          lumber = adjacent(&area, j, i, LUMBER);
          assert(wood + lumber <= 8);
          if (wood >= 1 && lumber >= 1) {
            row.push_back(LUMBER);
          } else {
            row.push_back(EMPTY);
          }
          break;
      }
    }
    next.push_back(row);
  }
  area = next;

  int wood = 0;
  int lumber = 0;
  for (auto row : area) {
    for (auto land : row) {
      switch (land) {
        case WOOD:
          wood++;
          break;
        case LUMBER:
          lumber++;
          break;
        case EMPTY:
          break;
      }
    }
  }

  return wood * lumber;
}

std::pair<int, int> matchFound(std::vector<int> &sequence, int minMatches) {
  int matches = 0;
  auto it = sequence.begin();
  auto it2 = sequence.end();
  it2--;

  while (it != it2) {
    if (*it != *it2) {
      it++;
    } else {
      assert(it != it2);
      do {
        matches++;
        it--;
        it2--;
      } while (matches < minMatches && *it == *it2);
      if (matches >= minMatches) {
        return std::make_pair(std::distance(sequence.begin(), it),
                              std::distance(it, it2));
      } else {
        it2 = sequence.end();
        it2--;
      }
    }
  }

  return std::make_pair(-1, 0);
}

void partOne(std::vector<std::vector<Acre>> puzzle) {
  std::vector<int> sequence;
  int minMatches = 20;
  int limit = 1000000000;
  for (int i = 0; i < limit; i++) { // We may need to continue until the end.
    sequence.push_back(step(puzzle));
    std::pair<int, int> result = matchFound(sequence, minMatches);
    if (result.first != -1) {
      int loopLength = result.second;
      int loopStartIndex = result.first;
      int index = (limit - loopStartIndex) % loopLength;
      std::cout << "Resource value after " << limit << " minutes: " << sequence[index] << "\n";
      return;
    }
  }

  int wood = 0;
  int lumber = 0;
  for (auto row : puzzle) {
    for (auto land : row) {
      switch (land) {
        case WOOD:
          wood++;
          break;
        case LUMBER:
          lumber++;
          break;
        case EMPTY:
          break;
      }
    }
  }

  std::cout << "Total score: " << wood << " * " << lumber << " = "
            << wood * lumber << "\n";
}

int main() {
  auto puzzle = readSequence();
  partOne(puzzle);
}
