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

long scoreFast(int players, int limit) {
  list<int> marbles = {0, 1};
  int position = 1;
  map<int, long> playerMarbles;
  auto it = marbles.begin();
  it++;

  for (int i = 2; i <= limit; i++) {
    int currentPlayer = i % players;
    int size = marbles.size();

    if (i % 23 == 0) {
      playerMarbles[currentPlayer] += i;
      int newPos = (size - 7 + position) % size;
      if (newPos > position) {
        it = marbles.end();
        position = size;
      }
      while (position != newPos) {
        it--;
        position--;
      }
      playerMarbles[currentPlayer] += *it;
      it = marbles.erase(it);
    } else {
      int newPos = (position + 2) % size;
      if (newPos == 0) {
        marbles.push_back(i);
        position = size;
        it = marbles.end();
      } else {
        if (newPos == 1) {
          it = marbles.begin();
          it++;
        } else {
          if (newPos == 2) {
            it = marbles.begin();
          }
          it++;
          it++;
        }
        it = marbles.insert(it, i);
        position = newPos;
      }
    }
  }

  long max = 0;
  for (auto [player, score] : playerMarbles) {
    if (score > max) {
      max = score;
    }
  }
  return max;
}

map<int, set<int>> score(int players, int limit) {
  vector<int> marbles;
  marbles.push_back(0);
  marbles.push_back(1);
  int position = 1;
  map<int, set<int>> playerMarbles;

  for (int i = 2; i <= limit; i++) {
    int currentPlayer = i % players;

    if (i % 23 == 0) {
      playerMarbles[currentPlayer].insert(i);
      position = (marbles.size() - 7 + position) % marbles.size();
      playerMarbles[currentPlayer].insert(marbles[position]);
      marbles.erase(marbles.begin() + position);
    } else {
      int newPos = (position + 2) % marbles.size();
      if (newPos == 0) {
        marbles.push_back(i);
        position = marbles.size() - 1;
      } else {
        marbles.insert(marbles.begin() + newPos, i);
        position = newPos;
      }
    }
  }
  return playerMarbles;
}

long maxScore(map<int, set<int>> scores) {
  long max = 0;
  int player = 0;
  for (auto [p, marbles] : scores) {
    long score = accumulate(marbles.begin(), marbles.end(), 0);
    if (score > max) {
      max = score;
    }
  }
  return max;
}

void partOne(int players, int limit) {
  cout << "Max score: " << maxScore(score(players, limit));
}

void partTwo(int players, int limit) {
  cout << "Max score: " << scoreFast(players, limit) << "\n";
}

int main() {
  cout << "==== Part One ====\n";
  partOne(418, 70769);
  cout << "\n==== Part Two ====\n";
  partTwo(418, 7076900);
}
