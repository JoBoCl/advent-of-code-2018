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

void step(std::vector<std::vector<Acre>> &area) {
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
}

void partOne(std::vector<std::vector<Acre>> puzzle) {
  for (int i = 0; i < 10; i++) {
    step(puzzle);
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
