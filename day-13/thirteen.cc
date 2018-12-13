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

enum Track { EMPTY, JUNCTION, TURN_L, TURN_R, VERT, HORIZ };

enum Turn { LEFT, FORWARD, RIGHT };

enum Direction { NORTH, SOUTH, EAST, WEST };

struct Location {
  int x;
  int y;
  bool isTrack;
  Track type;
  std::map<Direction, int> neighbours;

  bool isJunction() { return neighbours.size() == 4; }
  bool shouldTurn() { return type == TURN_L || type == TURN_R; }

  Direction change(Direction d) {
    assert(this->shouldTurn());
    for (auto [dir, i] : this->neighbours) {
      if (dir != d) {
        return dir;
      }
    }
    return d;
  }
};

bool operator==(const Location &l, const Location &r) {
  return l.x == r.x && l.y == r.y;
}

struct Cart {
  Turn nextTurn;
  Direction direction;
  int x;
  int y;

  bool crashed;

  /** Returns the current direction. */
  Direction currentDirection() { return direction; }

  /** Returns the current position. */
  std::pair<int, int> currentPosition() { return std::make_pair(x, y); }

  /** Returns the next position, and updates the current state. */
  void nextPosition() {
    switch (direction) {
    case NORTH:
      y--;
      return;
    case SOUTH:
      y++;
      return;
    case EAST:
      x++;
      return;
    case WEST:
      x--;
      return;
    }
  }

  /** Returns the next direction, and updates the current state. */
  Direction nextDirection() {
    switch (direction) {
    case NORTH:
      switch (nextTurn) {
      case LEFT:
        nextTurn = FORWARD;
        direction = WEST;
        return WEST;
      case FORWARD:
        nextTurn = RIGHT;
        return direction;
      case RIGHT:
        nextTurn = LEFT;
        direction = EAST;
        return EAST;
      }
    case EAST:
      switch (nextTurn) {
      case LEFT:
        nextTurn = FORWARD;
        direction = NORTH;
        return NORTH;
      case FORWARD:
        nextTurn = RIGHT;
        return direction;
      case RIGHT:
        nextTurn = LEFT;
        direction = SOUTH;
        return SOUTH;
      }
    case SOUTH:
      switch (nextTurn) {
      case LEFT:
        nextTurn = FORWARD;
        direction = EAST;
        return EAST;
      case FORWARD:
        nextTurn = RIGHT;
        return direction;
      case RIGHT:
        nextTurn = LEFT;
        direction = WEST;
        return WEST;
      }
    case WEST:
      switch (nextTurn) {
      case LEFT:
        nextTurn = FORWARD;
        direction = SOUTH;
        return SOUTH;
      case FORWARD:
        nextTurn = RIGHT;
        return direction;
      case RIGHT:
        nextTurn = LEFT;
        direction = NORTH;
        return NORTH;
      }
    }
  }
};

bool operator<(Cart const &left, Cart const &right) {
  if (left.y < right.y) {
    return true;
  }
  if (left.y > right.y) {
    return false;
  }

  return left.x < right.x;
}

struct Puzzle {
  std::map<int, Location> locations;
  std::map<int, Cart *> carts;
  int maxX;
  int maxY;

  int position(int x, int y) { return y * this->maxX + x; }

  Location location(int x, int y) {
    return this->locations[this->position(x, y)];
  }

  void print() {
    std::cout << "Carts on track: " << this->carts.size() << "\n";
    for (int i = 0; i < maxY; i++) {
      for (int j = 0; j < maxX; j++) {
        if (this->carts.count(this->position(j, i)) > 0) {
          switch (this->carts[this->position(j, i)]->direction) {
          case NORTH:
            std::cout << "^";
            break;
          case SOUTH:
            std::cout << "v";
            break;
          case WEST:
            std::cout << "<";
            break;
          case EAST:
            std::cout << ">";
            break;
          }

          continue;
        }
        switch (this->location(j, i).type) {
        case JUNCTION:
          std::cout << '+';
          break;

        case TURN_R:
          std::cout << '\\';
          break;
        case TURN_L:
          std::cout << '/';
          break;

        case VERT:
          std::cout << '|';
          break;
        case HORIZ:
          std::cout << '-';
          break;

        default:
        case EMPTY:
          std::cout << ' ';
          break;
        }
      }
      std::cout << "\n";
    }
  }
};

Puzzle readSequence() {
  std::ifstream myfile("13.input");
  Puzzle puzzle;
  std::string line;
  if (myfile.is_open()) {
    int y = 0;
    getline(myfile, line);
    puzzle.maxX = line.length() + 1;
    do {
      int x = 0;
      for (char ch : line) {
        auto l = &puzzle.locations[puzzle.position(x, y)];
        l->x = x;
        l->y = y;
        Cart *c = new Cart;
        Location predecessor;
        c->nextTurn = LEFT;
        c->crashed = false;
        bool cartFound = false;
        switch (ch) {
        case '+':
          l->isTrack = true;
          l->neighbours[NORTH] = puzzle.position(x, y - 1);
          l->neighbours[SOUTH] = puzzle.position(x, y + 1);
          l->neighbours[EAST] = puzzle.position(x + 1, y);
          l->neighbours[WEST] = puzzle.position(x - 1, y);
          l->type = JUNCTION;
          break;

        case '-':
          l->isTrack = true;
          l->neighbours[EAST] = puzzle.position(x + 1, y);
          l->neighbours[WEST] = puzzle.position(x - 1, y);
          l->type = HORIZ;
          break;

        case '|':
          l->isTrack = true;
          l->neighbours[NORTH] = puzzle.position(x, y - 1);
          l->neighbours[SOUTH] = puzzle.position(x, y + 1);
          l->type = VERT;
          break;

        case '\\':
          l->isTrack = true;
          // |
          // \-
          predecessor = puzzle.locations[puzzle.position(x, y - 1)];
          if (predecessor.isTrack &&
              (predecessor.type == VERT || predecessor.type == JUNCTION)) {
            l->neighbours[NORTH] = puzzle.position(x, y - 1);
            l->neighbours[EAST] = puzzle.position(x + 1, y);
          } else {
            // -\
	    //  |
            assert((puzzle.locations[puzzle.position(x - 1, y)].isTrack));
            l->neighbours[SOUTH] = puzzle.position(x, y + 1);
            l->neighbours[WEST] = puzzle.position(x - 1, y);
          }
          l->type = TURN_R;
          break;

        case '/':
          l->isTrack = true;
          l->type = TURN_L;
          //  |
          // -/
          predecessor = puzzle.locations[puzzle.position(x, y - 1)];
          if (predecessor.isTrack &&
              (predecessor.type == VERT || predecessor.type == JUNCTION)) {
            l->neighbours[NORTH] = puzzle.position(x, y - 1);
            l->neighbours[WEST] = puzzle.position(x - 1, y);
          } else {
            // /-
            // |
            // cassert((puzzle.locations[puzzle.position(x, y - 1)].isTrack));
            l->neighbours[SOUTH] = puzzle.position(x, y + 1);
            l->neighbours[EAST] = puzzle.position(x + 1, y);
          }
          break;

          // Assume that carts never start on junctions.
        case '>':
          c->direction = EAST;
          c->x = x;
          c->y = y;
          puzzle.carts[puzzle.position(x, y)] = c;
          l->isTrack = true;
          l->neighbours[EAST] = puzzle.position(x + 1, y);
          l->neighbours[WEST] = puzzle.position(x - 1, y);
          l->type = HORIZ;
          cartFound = true;
          break;

        case '<':
          c->direction = WEST;
          c->x = x;
          c->y = y;
          puzzle.carts[puzzle.position(x, y)] = c;
          l->isTrack = true;
          l->neighbours[EAST] = puzzle.position(x + 1, y);
          l->neighbours[WEST] = puzzle.position(x - 1, y);
          l->type = HORIZ;
          cartFound = true;
          break;

        case '^':
          c->direction = NORTH;
          c->x = x;
          c->y = y;
          puzzle.carts[puzzle.position(x, y)] = c;
          l->isTrack = true;
          l->neighbours[NORTH] = puzzle.position(x, y - 1);
          l->neighbours[SOUTH] = puzzle.position(x, y + 1);
          l->type = VERT;
          cartFound = true;
          break;

        case 'v':
          c->direction = SOUTH;
          c->x = x;
          c->y = y;
          puzzle.carts[puzzle.position(x, y)] = c;
          l->isTrack = true;
          l->neighbours[NORTH] = puzzle.position(x, y - 1);
          l->neighbours[SOUTH] = puzzle.position(x, y + 1);
          l->type = VERT;
          cartFound = true;
          break;

        default:
        case ' ':
          l->isTrack = false;
          l->type = EMPTY;
          break;
        }
        if (!cartFound) {
          delete c;
        }

        x++;
      }
      y++;
    } while (getline(myfile, line));
    puzzle.maxY = y;
  }

  return puzzle;
}

bool tick(Puzzle *puzzle) {
  bool crashed = false;
  std::map<int, std::set<Cart *>> cartsByPosition;
  // Ordered by position.
  for (auto [pos, cart] : puzzle->carts) {
    if (cart->crashed) {
      delete cart;
      continue;
    }

    auto currentLocation = puzzle->locations[pos];
    Direction oldDir = cart->currentDirection();
    Direction dir = cart->currentDirection();
    if (currentLocation.isJunction()) {
      dir = cart->nextDirection();
    } else if (currentLocation.shouldTurn()) {
      dir = currentLocation.change(dir);
      cart->direction = dir;
    }
    assert(currentLocation.neighbours.count(dir) == 1);
    auto nextLocation = puzzle->locations[currentLocation.neighbours[dir]];
    assert(nextLocation.isTrack);
    cart->nextPosition();
    assert((puzzle->location(cart->x, cart->y) == nextLocation));
    if (puzzle->carts.count(puzzle->position(cart->x, cart->y)) ==
        1) { // Collision before other car moved.
      crashed = true;
      std::cout << "Crash at: " << cart->x << ", " << cart->y << "\n";
      puzzle->carts[puzzle->position(cart->x, cart->y)]->crashed = true;
    } else {
      cartsByPosition[puzzle->position(cart->x, cart->y)].insert(cart);
    }
  }

  puzzle->carts.clear();

  std::cout << "Unique positions with carts: " << cartsByPosition.size()
            << ".\n";

  for (auto [pos, cs] : cartsByPosition) {
    if (cs.size() == 1) {
      auto c = *cs.begin();
      puzzle->carts[pos] = c;
    } else {
      crashed = true;
      auto c = *cs.begin();
      std::cout << "Crash at: " << c->x << ", " << c->y << "\n";

      for (auto it = cs.begin(); it != cs.end(); it++) {
        delete *it;
      }
    }
  }

  return crashed;
}

int main() {
  auto puzzle = readSequence();
  puzzle.print();
  int i = 0;
  while (!tick(&puzzle)) {
    // puzzle.print();
    std::cout << "Iteration " << ++i << " completed.\n";
  }
}
