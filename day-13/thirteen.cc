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

enum Direction { NORTH, EAST, SOUTH, WEST };

Direction opposite(Direction d) { return static_cast<Direction>((d + 2) % 4); }

struct Location {
  Location() : x(-1), y(-1), isTrack(false), type(EMPTY) {}

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
      if (dir != d && opposite(d) != dir) {
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
  Cart() : nextTurn(LEFT), crashed(false) {}

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
      case EAST:
        x++;
        return;
      case SOUTH:
        y++;
        return;
      case WEST:
        x--;
        return;
    }
  }

  /** Returns the next direction, and updates the current state. */
  Direction nextDirection() {
    switch (nextTurn) {
      case LEFT:
        direction = static_cast<Direction>((direction + 3) % 4);
        nextTurn = FORWARD;
        break;
      case FORWARD:
        nextTurn = RIGHT;
        break;
      case RIGHT:
        direction = static_cast<Direction>((direction + 1) % 4);
        nextTurn = LEFT;
        break;
    }
    return direction;
  }
};

struct Puzzle {
  std::map<int, Location> locations;
  std::map<int, Cart *> carts;
  int maxX;
  int maxY;

  int position(int x, int y) { return y * this->maxX + x; }

  Location location(int p) {
    auto loc = this->locations.at(p);
    assert(loc.x != -1 && loc.y != -1);
    return loc;
  }

  Location location(int x, int y) {
    return this->location(this->position(x, y));
  }

  void print() {
    std::cout << "Carts on track: " << this->carts.size() << "\n";
    for (int i = 0; i < maxY; i++) {
      for (int j = 0; j < maxX; j++) {
        int pos = this->position(j, i);
        if (this->carts.count(pos) > 0) {
          switch (this->carts[pos]->direction) {
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

        if (this->locations.count(pos) == 0) {
          std::cout << ' ';
          continue;
        }

        switch (this->locations[pos].type) {
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
        Location l;
        l.x = x;
        l.y = y;
        Cart *c = new Cart;
        Location predecessor;
        bool cartFound = false;
        bool trackFound = true;
        switch (ch) {
          case '+':
            l.isTrack = true;
            l.neighbours[NORTH] = puzzle.position(x, y - 1);
            l.neighbours[SOUTH] = puzzle.position(x, y + 1);
            l.neighbours[EAST] = puzzle.position(x + 1, y);
            l.neighbours[WEST] = puzzle.position(x - 1, y);
            l.type = JUNCTION;
            break;

          case '-':
            l.isTrack = true;
            l.neighbours[EAST] = puzzle.position(x + 1, y);
            l.neighbours[WEST] = puzzle.position(x - 1, y);
            l.type = HORIZ;
            break;

          case '|':
            l.isTrack = true;
            l.neighbours[NORTH] = puzzle.position(x, y - 1);
            l.neighbours[SOUTH] = puzzle.position(x, y + 1);
            l.type = VERT;
            break;

          case '\\':
            l.isTrack = true;
            // |
            // \-
            predecessor = puzzle.locations[puzzle.position(x, y - 1)];
            if (predecessor.isTrack &&
                (predecessor.type == VERT || predecessor.type == JUNCTION)) {
              l.neighbours[NORTH] = puzzle.position(x, y - 1);
              l.neighbours[EAST] = puzzle.position(x + 1, y);
            } else {
              // -\ 
	          //  |
              assert((puzzle.locations[puzzle.position(x - 1, y)].isTrack));
              l.neighbours[SOUTH] = puzzle.position(x, y + 1);
              l.neighbours[WEST] = puzzle.position(x - 1, y);
            }
            l.type = TURN_R;
            break;

          case '/':
            l.isTrack = true;
            l.type = TURN_L;
            //  |
            // -/
            predecessor = puzzle.locations[puzzle.position(x, y - 1)];
            if (predecessor.isTrack &&
                (predecessor.type == VERT || predecessor.type == JUNCTION)) {
              l.neighbours[NORTH] = puzzle.position(x, y - 1);
              l.neighbours[WEST] = puzzle.position(x - 1, y);
            } else {
              // /-
              // |
              // cassert((puzzle.locations[puzzle.position(x, y - 1)].isTrack));
              l.neighbours[SOUTH] = puzzle.position(x, y + 1);
              l.neighbours[EAST] = puzzle.position(x + 1, y);
            }
            break;

            // Assume that carts never start on junctions.
          case '>':
            c->direction = EAST;
            c->x = x;
            c->y = y;
            puzzle.carts[puzzle.position(x, y)] = c;
            l.isTrack = true;
            l.neighbours[EAST] = puzzle.position(x + 1, y);
            l.neighbours[WEST] = puzzle.position(x - 1, y);
            l.type = HORIZ;
            cartFound = true;
            break;

          case '<':
            c->direction = WEST;
            c->x = x;
            c->y = y;
            puzzle.carts[puzzle.position(x, y)] = c;
            l.isTrack = true;
            l.neighbours[EAST] = puzzle.position(x + 1, y);
            l.neighbours[WEST] = puzzle.position(x - 1, y);
            l.type = HORIZ;
            cartFound = true;
            break;

          case '^':
            c->direction = NORTH;
            c->x = x;
            c->y = y;
            puzzle.carts[puzzle.position(x, y)] = c;
            l.isTrack = true;
            l.neighbours[NORTH] = puzzle.position(x, y - 1);
            l.neighbours[SOUTH] = puzzle.position(x, y + 1);
            l.type = VERT;
            cartFound = true;
            break;

          case 'v':
            c->direction = SOUTH;
            c->x = x;
            c->y = y;
            puzzle.carts[puzzle.position(x, y)] = c;
            l.isTrack = true;
            l.neighbours[NORTH] = puzzle.position(x, y - 1);
            l.neighbours[SOUTH] = puzzle.position(x, y + 1);
            l.type = VERT;
            cartFound = true;
            break;

          default:
          case ' ':
            l.isTrack = false;
            l.type = EMPTY;
            trackFound = false;
            break;
        }

        if (!cartFound) {
          delete c;
        }

        if (trackFound) {
          puzzle.locations[puzzle.position(x, y)] = l;
        }

        x++;
      }
      y++;
    } while (getline(myfile, line));
    puzzle.maxY = y;
  }

  return puzzle;
}

std::pair<bool, int> tick(Puzzle *puzzle) {
  bool crashed = false;
  std::map<int, Cart *> cartsByPosition;
  // Ordered by position.
  for (auto [pos, cart] : puzzle->carts) {
    if (cart->crashed) {
      delete cart;
      continue;
    }

    auto currentLocation = puzzle->location(pos);
    Direction oldDir = cart->currentDirection();
    Direction dir = cart->currentDirection();
    if (currentLocation.isJunction()) {
      dir = cart->nextDirection();
    } else if (currentLocation.shouldTurn()) {
      dir = currentLocation.change(dir);
      cart->direction = dir;
    }
    assert(currentLocation.neighbours.count(dir) == 1);
    auto nextLocation = puzzle->location(currentLocation.neighbours[dir]);
    assert(nextLocation.isTrack);
    cart->nextPosition();
    int newIndex = puzzle->position(cart->x, cart->y);
    assert((puzzle->location(cart->x, cart->y) == nextLocation));
    // Collision before other car moved.
    if (puzzle->carts.count(newIndex) == 1) {
      auto other = puzzle->carts[newIndex];
      if (other->x == cart->x && other->y == cart->y) {
        crashed = true;
        std::cout << "Crash at: " << cart->x << ", " << cart->y << "\n";
        puzzle->carts[newIndex]->crashed = true;
        delete cart;
      } else {
        cartsByPosition[newIndex] = cart;
      }
    } else if (cartsByPosition.count(newIndex) == 1) {
      crashed = true;
      std::cout << "Crash at: " << cart->x << ", " << cart->y << "\n";
      delete cart;
      cartsByPosition.erase(newIndex);
    } else {
      cartsByPosition[newIndex] = cart;
    }
  }

  puzzle->carts.clear();

  puzzle->carts = cartsByPosition;

  return std::make_pair(crashed, cartsByPosition.size());
}

int main() {
  auto puzzle = readSequence();
  int i = 0;
  while (!tick(&puzzle).first) {
    // puzzle.print();
    //  std::cout << "Iteration " << ++i << " completed.\n";
  }
  while (tick(&puzzle).second != 1) {
  }
  auto lastRemaining = puzzle.carts.begin();
  std::cout << "Last remaining cart at " << lastRemaining->second->x << ", "
            << lastRemaining->second->y;
}
