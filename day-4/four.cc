#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

enum Action { SHIFT, WAKE, SLEEP };

bool operator<(const std::tm &t1, const std::tm &t2) {
  if (t1.tm_year != t2.tm_year) {
    return t1.tm_year < t2.tm_year;
  }
  if (t1.tm_mon != t2.tm_mon) {
    return t1.tm_mon < t2.tm_mon;
  }
  if (t1.tm_mday != t2.tm_mday) {
    return t1.tm_mday < t2.tm_mday;
  }
  if (t1.tm_hour != t2.tm_hour) {
    return t1.tm_hour < t2.tm_hour;
  }
  if (t1.tm_min != t2.tm_min) {
    return t1.tm_min < t2.tm_min;
  }
  return false;
};

int main() {
  map<tm, pair<Action, int>> actions;
  string line;
  std::ifstream myfile("4.input");
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      std::regex guardActivity(
          "\\[(\\d{4})-(\\d{2})-(\\d{2}) (\\d{2}):(\\d{2})\\] (falls "
          "asleep|wakes up|Guard #(\\d+) begins shift)");
      smatch result;
      bool match = regex_match(line, result, guardActivity);
      if (match) {
        int y = stoi(result[1].str());
        int m = stoi(result[2].str());
        int d = stoi(result[3].str());
        int h = stoi(result[4].str());
        int min = stoi(result[5].str());
        struct tm time = {.tm_min = min,
                          .tm_hour = h,
                          .tm_mday = d,
                          .tm_mon = m,
                          .tm_year = y - 1500};
        // time_t t = mktime(&time);

        Action action;
        int guard = -1;
        if (result[6].str() == "falls asleep") {
          action = SLEEP;
        } else if (result[6].str() == "wakes up") {
          action = WAKE;
        } else if (result[6].str()[0] == 'G') {
          action = SHIFT;
          guard = stoi(result[7].str());
        } else {
          cout << result[6];
          continue;
        }
        actions[time] = make_pair(action, guard);
      } else {
        cout << line;
      }
    }
  }

  int lastGuard = -1;
  for (auto [time, action] : actions) {
    if (action.first == SHIFT) {
      lastGuard = action.second;
    } else {
      action.second = lastGuard;
    }
  }

  map<int, int> sleepTime;
  map<int, map<int, int>> guardSleepByMinute;
  int guard = -1;
  int totalSleep;
  tm lastTime;
  for (auto [time, action] : actions) {
    switch (action.first) {
    case SHIFT:
      if (guard != -1) {
        sleepTime[guard] += totalSleep;
      }
      guard = action.second;
      totalSleep = 0;
      break;
    case SLEEP:
      break;
    case WAKE:
      totalSleep += time.tm_min - lastTime.tm_min;
      for (int i = lastTime.tm_min; i < time.tm_min; i++) {
        guardSleepByMinute[guard][i]++;
      }
    }
    lastTime = time;
  }

  int max = -1;
  int sleepiest = -1;
  for (auto [guard, sleep] : sleepTime) {
    if (sleep > max) {
      sleepiest = guard;
      max = sleep;
    }
  }
  cout << "Sleepiest guard: " << sleepiest << " slept for " << max
       << " minutes."
       << "\n";

  /**
  map<int, int> sleepCount;
  for (auto [time, action] : actions) {
    if (action.second == sleepiest) {
      cout << asctime(&time) << "Guard " << action.second << " does "
           << action.first;
      if (action.first == WAKE) {
        int sleepInterval = abs(lastTime.tm_min - time.tm_min);
        int startMin = lastTime.tm_min;
        cout << "Start min: " << startMin;
        while (sleepInterval > 0) {
          sleepCount[startMin + sleepInterval]++;
          sleepInterval--;
        }
      }
    }
    lastTime = time;
  }
  */

  max = -1;
  int sleepiestMin = -1;

  for (auto [minute, count] : guardSleepByMinute[sleepiest]) {
    if (max < count) {
      sleepiestMin = minute;
      max = count;
    }
  }

  cout << "Sleepiest minute: " << sleepiestMin << "\n";

  cout << "Checksum 1: " << sleepiestMin * sleepiest << "\n";
}
