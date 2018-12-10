#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
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

using namespace std;

struct Star {
  int x;
  int y;
  int u;
  int v;
};

struct Box {
  int x0;
  int x1;
  int y0;
  int y1;
};

int area(struct Box *b) { return (b->x1 - b->x0) * (b->y1 - b->y0); }
int perimeter(struct Box *b) { return (b->x1 - b->x0) + (b->y1 - b->y0); }

bool lessThan(struct Box *l, struct Box *r) {
  return perimeter(l) < perimeter(r);
}

vector<struct Star> readSequence() {
  vector<struct Star> stars;
  std::ifstream myfile("10.input");
  string line;
  regex matcher("position=<\\s*(-?\\d+),\\s*(-?\\d+)> "
                "velocity=<\\s*(-?\\d+),\\s*(-?\\d+)>");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      struct Star star;
      smatch match;
      if (regex_match(line, match, matcher)) {
        struct Star star;
        star.x = stoi(match[1].str());
        star.y = stoi(match[2].str());
        star.u = stoi(match[3].str());
        star.v = stoi(match[4].str());
        stars.push_back(star);
      }
    }
  }

  return stars;
}

Box maxSize(vector<Star> *stars) {
  struct Box box;
  box.x0 = numeric_limits<int>::max();
  box.y0 = numeric_limits<int>::max();
  box.x1 = numeric_limits<int>::min();
  box.y1 = numeric_limits<int>::min();

  for (Star &star : *stars) {
    if (star.x < box.x0)
      box.x0 = star.x;
    if (star.y < box.y0)
      box.y0 = star.y;
    if (star.x > box.x1)
      box.x1 = star.x;
    if (star.y > box.y1)
      box.y1 = star.y;
  }
  return box;
}

void advance(Star *star) {
  star->x += star->u;
  star->y += star->v;
}

void retreat(Star *star) {
  star->x -= star->u;
  star->y -= star->v;
}

void partOne(vector<Star> stars) {
  struct Box oldBounds;
  struct Box newBounds = maxSize(&stars);
  int iterations = 0;

  do {
    oldBounds = newBounds;
    for (auto &star : stars) {
      advance(&star);
    }
    newBounds = maxSize(&stars);
    iterations++;
  } while (lessThan(&newBounds, &oldBounds));

  cout << "Current bounds: " << oldBounds.x1 - oldBounds.x0 << ", "
       << oldBounds.y1 - oldBounds.y0 << " after " << iterations - 1
       << " iterations.  Print [y/N]?";

  char print = 'Y';
  // cin >> print;

  for (auto &star : stars) {
    retreat(&star);
  }

  if (toupper(print) == 'Y') {
    for (int j = oldBounds.y0; j <= oldBounds.y1; j++) {
      for (int i = oldBounds.x0; i <= oldBounds.x1; i++) {
        bool print = false;
        for (auto &star : stars) {
          if (star.x == i && star.y == j) {
            print = true;
            break;
          }
        }

        cout << (print ? '#' : ' ');
      }
      cout << "\n";
    }
  }
}

void partTwo(vector<Star> stars) {}

int main() {
  vector<struct Star> node = readSequence();
  cout << "==== Part One ====\n";
  partOne(node);
  cout << "\n==== Part Two ====\n";
  partTwo(node);
}
