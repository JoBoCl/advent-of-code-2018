#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

using namespace std;

struct Node {
  vector<Node> child;
  vector<int> metadata;
};

Node parseNode(vector<int> *results) {
  int children = *(results->begin());
  results->erase(results->begin());
  int metadata = *results->begin();
  results->erase(results->begin());

  struct Node node;

  for (int i = 0; i < children; i++) {
    node.child.push_back(parseNode(results));
  }

  for (int i = 0; i < metadata; i++) {
    node.metadata.push_back(*(results->begin()));
    results->erase(results->begin());
  }

  return node;
}

Node readSequence() {
  struct Node node;
  std::ifstream myfile("8.input");
  string line;
  if (myfile.is_open()) {
    getline(myfile, line);
    istringstream iss(line);
    vector<string> tokens((istream_iterator<string>(iss)),
                          istream_iterator<string>());
    vector<int> results;
    transform(tokens.begin(), tokens.end(), back_inserter(results),
              [](string s) -> int { return stoi(s); });
    node = parseNode(&results);
  }

  return node;
}

int metadataSum(struct Node *node) {
  int nodeSum = accumulate(node->metadata.begin(), node->metadata.end(), 0);
  vector<int> childSums;
  transform(node->child.begin(), node->child.end(), back_inserter(childSums),
            [](struct Node node) { return metadataSum(&node); });
  return accumulate(childSums.begin(), childSums.end(), nodeSum);
}

void partOne(struct Node *node) { cout << metadataSum(node); }

void partTwo(struct Node *node) {}

int main() {
  struct Node node = readSequence();
  cout << "==== Part One ====\n";
  partOne(&node);
  cout << "\n==== Part Two ====\n";
  partTwo(&node);
}
