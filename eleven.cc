#include "cstdio"
#include "iostream"
#include "limits"
#include "utility"
#include "vector"

int powerLevel(std::vector<std::vector<int>>* power, int x, int y, int n) {
  int limit = (n - 1) / 2;
  if (x - limit < 0 || x + limit > 299 || y - limit < 0 || y + limit > 299) {
    return std::numeric_limits<int>::min();
  }
  int sum = 0;
  for (int i = -limit; i <= limit; i++) {
    for (int j = -limit; j <= limit; j++) {
      sum += (*power).at(y + j).at(x + i);
    }
  }
  return sum;
}

int powerLevel(std::vector<std::vector<int>>* power, int x, int y) {
  return powerLevel(power, x, y, 3);
}

void printSegment(std::vector<std::vector<int>>* power, int x, int y) {
  for (int j = y - 2; j < y + 3 && j < 299; j++) {
    for (int i = x - 2; i < x + 3 && i < 299; i++) {
      printf("%3d", (*power)[j][i]);
    }
    std::cout << "\n";
  }
}

void partOne(int serialNo) {
  std::vector<std::vector<int>> power;

  for (int i = 1; i <= 300; i++) {  // y
    std::vector<int> row;
    for (int j = 1; j <= 300; j++) {  // x
      int rackId = j + 10;
      int powerLevel = rackId * i;
      powerLevel += serialNo;
      powerLevel *= rackId;
      powerLevel = ((powerLevel / 100) % 10);
      row.push_back(powerLevel - 5);
    }
    power.push_back(row);
  }

  std::pair<int, int> coords;
  int maxPower = std::numeric_limits<int>::min();
  for (int i = 1; i <= 300; i++) {
    for (int j = 1; j <= 300; j++) {
      int level = powerLevel(&power, i, j);
      if (level > maxPower) {
        coords = std::make_pair(i, j);
        maxPower = level;
      }
    }
  }

  printSegment(&power, coords.first, coords.second);
  std::cout << "Maximum power, n=3: (" << coords.first << ", " << coords.second
            << ").\n";

  int size = 1;
  maxPower = std::numeric_limits<int>::min();
  for (int n = 1; n <= 300; n++) {
    for (int i = 1; i <= 300; i++) {
      for (int j = 1; j <= 300; j++) {
        int level = powerLevel(&power, i, j, n);
        if (level > maxPower) {
          coords = std::make_pair(i, j);
          maxPower = level;
          size = n;
        }
      }
    }
  }
  std::cout << "Maximum power, n=" << size << ": (" << coords.first << ", "
            << coords.second << ").\n";
}

int main() { partOne(9798); }
