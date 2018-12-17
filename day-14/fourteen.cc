#include <iostream>
#include <list>
#include <vector>

void step(std::list<short> *input, std::list<short>::iterator &elfOne,
          std::list<short>::iterator &elfTwo) {
  int sum = *elfOne + *elfTwo;
  int oneAdvance = 1 + *elfOne;
  int twoAdvance = 1 + *elfTwo;

  if (sum >= 10) {
    input->insert(input->end(), sum / 10);
  }
  input->insert(input->end(), sum % 10);

  while (oneAdvance > 0 ) {
    elfOne++; oneAdvance--;
    if (elfOne == input->end()) {
      elfOne = input->begin();
    }
  }

  while (twoAdvance > 0 ) {
    elfTwo++; twoAdvance--;
    if (elfTwo == input->end()) {
      elfTwo = input->begin();
    }
  }
}

std::pair<int, int> step(std::vector<short> &input, int elfOne, int elfTwo) {
  int sum = input.at(elfOne) + input.at(elfTwo);
  if (sum >= 10) {
    input.push_back(sum / 10);
  }
  input.push_back(sum % 10);

  elfOne = (elfOne + 1 + input.at(elfOne)) % input.size();
  elfTwo = (elfTwo + 1 + input.at(elfTwo)) % input.size();
  return std::make_pair(elfOne, elfTwo);
}

void partOne(std::vector<short> input, int iterations) {
  int elfOne = 0;
  int elfTwo = 1;

  while (input.size() < iterations + 10) {
    auto elves = step(input, elfOne, elfTwo);
    elfOne = elves.first;
    elfTwo = elves.second;
  }

  int initialLength = input.size();

  for (int i = iterations; i < iterations + 10; i++) std::cout << input.at(i);
  std::cout << "\n";
}

// Returns the digits in reverse order.
std::vector<short> digits(int value) {
  std::vector<short> v;
  while (value != 0) {
    v.push_back(value % 10);
    value /= 10;
  }
  return v;
}

bool endsWith(std::list<short> *sequence, std::vector<short> *target, bool skipFirst) {
  auto seqIt = sequence->rbegin();
  auto targIt = target->begin();
  if (skipFirst) {
    seqIt++;
  }
  while (seqIt != sequence->rend() && targIt != target->end()) {
    if (*seqIt != *targIt) {
      return false;
    }
    seqIt++;
    targIt++;
  }
  return target->end() == targIt;
}


void partTwo(std::list<short> input, int target) {
  auto elfOne = input.begin();
  auto elfTwo = input.begin();
  elfTwo++;

  auto targetVector = digits(target);
  int iterations = 0;
  while (!endsWith(&input, &targetVector, true) && !endsWith(&input, &targetVector, false)) {
    step(&input, elfOne, elfTwo);
  }

  std::cout << "Prior recipes: " << (input.size() - targetVector.size())
            << "\n";
}

int main() {
  std::vector<short> input = {3, 7};

  std::list<short> inputList = {3, 7};

  partOne(input, 637061);

  partTwo(inputList, 51589);
  partTwo(inputList, 92510);
  partTwo(inputList, 59414);

  partTwo(inputList, 637061);
}