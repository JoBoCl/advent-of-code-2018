#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <utility>
#include <vector>

enum OpCode {
  ADDR,
  ADDI,
  MULR,
  MULI,
  BANR,
  BANI,
  BORR,
  BORI,
  SETR,
  SETI,
  GTIR,
  GTRI,
  GTRR,
  EQIR,
  EQRI,
  EQRR,
};

std::vector<OpCode> ALL_OPERATIONS({
    ADDR,
    ADDI,
    MULR,
    MULI,
    BANR,
    BANI,
    BORR,
    BORI,
    SETR,
    SETI,
    GTIR,
    GTRI,
    GTRR,
    EQIR,
    EQRI,
    EQRR,
});

struct Operation {
  std::array<int, 4> before;

  int opcode;
  int input1;
  int input2;
  int output;

  std::vector<OpCode> candidateOperations;

  std::array<int, 4> after;

  Operation() {}
};

struct Instruction {
  OpCode opcode;
  int input1;
  int input2;
  int output;
};

void mutate(Instruction instruction, std::array<int, 4>& registers) {
  int input1 = instruction.input1;
  int input2 = instruction.input2;
  int output = instruction.output;
  switch (instruction.opcode) {
    case ADDR:
      std::cout << "ADDR " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] + registers[input2];
      break;
    case ADDI:
      std::cout << "ADDI " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] + input2;
      break;
    case MULR:
      std::cout << "MULR " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] * registers[input2];
      break;
    case MULI:
      std::cout << "MULI " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] * input2;
      break;
    case BANR:
      std::cout << "BANR " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] & registers[input2];
      break;
    case BANI:
      std::cout << "BANI " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] & input2;
      break;
    case BORR:
      std::cout << "BORR " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] | registers[input2];
      break;
    case BORI:
      std::cout << "BORI " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1] | input2;
      break;
    case SETR:
      std::cout << "SETR " << input1 << " " << input2 << " " << output;
      registers[output] = registers[input1];
      break;
    case SETI:
      std::cout << "SETI " << input1 << " " << input2 << " " << output;
      registers[output] = input1;
      break;
    case GTIR:
      std::cout << "GTIR " << input1 << " " << input2 << " " << output;
      {
        bool matched = (input1 > registers[input2]);
        registers[output] = matched ? 1 : 0;
      }
      break;
    case GTRI:
      std::cout << "GTRI " << input1 << " " << input2 << " " << output;
      {
        bool matched = (registers[input1] > input2);
        registers[output] = matched ? 1 : 0;
      }
      break;
    case GTRR:
      std::cout << "GTRR " << input1 << " " << input2 << " " << output;
      {
        bool matched = (registers[input1] > registers[input2]);
        registers[output] = matched ? 1 : 0;
      }
      break;
    case EQIR:
      std::cout << "EQIR " << input1 << " " << input2 << " " << output;
      {
        bool matched = (input1 == registers[input2]);
        registers[output] = matched ? 1 : 0;
      }
      break;
    case EQRI:
      std::cout << "EQRI " << input1 << " " << input2 << " " << output;
      {
        bool matched = (registers[input1] == input2);
        registers[output] = matched ? 1 : 0;
      }
      break;
    case EQRR:
      std::cout << "EQRR " << input1 << " " << input2 << " " << output;
      {
        bool matched = (registers[input1] == registers[input2]);
        registers[output] = matched ? 1 : 0;
        break;
      }
  }
  std::cout << "  [";
  for (auto r : registers) {
    std::cout << r << ", ";
  }
  std::cout << "]\n";
}

bool behavesLike(Operation* operation, OpCode opCode) {
  for (int i = 0; i < 4; i++) {
    if (i != operation->output) {
      assert(operation->before[i] == operation->after[i]);
    }
  }

  switch (opCode) {
    case ADDR:
      return (operation->before[operation->input1] +
              operation->before[operation->input2]) ==
             operation->after[operation->output];
    case ADDI:
      return (operation->before[operation->input1] + operation->input2) ==
             operation->after[operation->output];
    case MULR:
      return (operation->before[operation->input1] *
              operation->before[operation->input2]) ==
             operation->after[operation->output];
    case MULI:
      return (operation->before[operation->input1] * operation->input2) ==
             operation->after[operation->output];
    case BANR:
      return (operation->before[operation->input1] &
              operation->before[operation->input2]) ==
             operation->after[operation->output];
    case BANI:
      return (operation->before[operation->input1] & operation->input2) ==
             operation->after[operation->output];
    case BORR:
      return (operation->before[operation->input1] |
              operation->before[operation->input2]) ==
             operation->after[operation->output];
    case BORI:
      return (operation->before[operation->input1] | operation->input2) ==
             operation->after[operation->output];
    case SETR:
      return operation->before[operation->input1] ==
             operation->after[operation->output];
    case SETI:
      return operation->input1 == operation->after[operation->output];
    case GTIR: {
      bool matched = (operation->input1 > operation->before[operation->input2]);
      return matched ? (operation->after[operation->output] == 1)
                     : (operation->after[operation->output] == 0);
    }
    case GTRI: {
      bool matched = (operation->before[operation->input1] > operation->input2);
      return matched ? (operation->after[operation->output] == 1)
                     : (operation->after[operation->output] == 0);
    }
    case GTRR: {
      bool matched = (operation->before[operation->input1] >
                      operation->before[operation->input2]);
      return matched ? (operation->after[operation->output] == 1)
                     : (operation->after[operation->output] == 0);
    }
    case EQIR: {
      bool matched =
          (operation->input1 == operation->before[operation->input2]);
      return matched ? (operation->after[operation->output] == 1)
                     : (operation->after[operation->output] == 0);
    }
    case EQRI: {
      bool matched =
          (operation->before[operation->input1] == operation->input2);
      return matched ? (operation->after[operation->output] == 1)
                     : (operation->after[operation->output] == 0);
    }
    case EQRR: {
      bool matched = (operation->before[operation->input1] ==
                      operation->before[operation->input2]);
      return matched ? (operation->after[operation->output] == 1)
                     : (operation->after[operation->output] == 0);
    }
  }
}

std::set<OpCode> candidateCodes(Operation* operation) {
  std::set<OpCode> candidates;
  for (auto code : ALL_OPERATIONS) {
    if (behavesLike(operation, code)) {
      candidates.insert(code);
    }
  }
  return candidates;
}

std::vector<Operation*> readSequence() {
  std::ifstream myfile("16.1.input");

  std::regex before("Before: \\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");
  std::regex ops("(\\d+) (\\d+) (\\d+) (\\d+)");
  std::regex after("After:  \\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");

  std::vector<Operation*> operations;
  std::string line;
  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      Operation* op = new Operation();
      std::smatch match;
      assert((std::regex_search(line, match, before)));
      for (int index = 1; index <= 4; index++) {
        op->before[index - 1] = std::stoi(match[index].str());
      }
      assert((getline(myfile, line)));
      assert((std::regex_search(line, match, ops)));
      op->opcode = std::stoi(match[1].str());
      op->input1 = std::stoi(match[2].str());
      op->input2 = std::stoi(match[3].str());
      op->output = std::stoi(match[4].str());

      assert((getline(myfile, line)));
      assert((std::regex_search(line, match, after)));
      for (int index = 1; index <= 4; index++) {
        op->after[index - 1] = std::stoi(match[index].str());
      }

      operations.push_back(op);
      getline(myfile, line);
    }
  }

  return operations;
}

std::map<int, OpCode> partOne(std::vector<Operation*>& operations) {
  int count = 0;
  std::multimap<int, OpCode> opCodeCandidates;
  int i = 0;
  for (auto op : operations) {
    auto codes = candidateCodes(op);
    if (codes.size() >= 3) {
      count++;
    }
    if (opCodeCandidates.count(op->opcode) > 0) {
      auto [start, end] = opCodeCandidates.equal_range(op->opcode);
      while (start != end) {
        if (codes.count(start->second) != 1) {
          start = opCodeCandidates.erase(start);
        } else {
          start++;
        }
      }
    } else {
      for (auto code : codes) {
        opCodeCandidates.insert(std::make_pair(op->opcode, code));
      }
    }
  }

  std::cout << "Operations with more than 3 candidate opcodes: " << count
            << "\n";

  std::map<int, OpCode> codeMap;

  while (opCodeCandidates.size() > 0) {
    for (int i = 0; i < 16; i++) {
      if (opCodeCandidates.count(i) == 1) {
        auto [start, end] = opCodeCandidates.equal_range(i);
        OpCode code = start->second;
        codeMap[i] = code;
        opCodeCandidates.erase(i);
        auto it = opCodeCandidates.begin();
        while (it != opCodeCandidates.end()) {
          if (it->second == code) {
            it = opCodeCandidates.erase(it);
          } else {
            it++;
          }
        }
      }
    }
  }

  assert(opCodeCandidates.size() == 0);
  return codeMap;
}

void partTwo(std::map<int, OpCode> codeMap) {
  std::ifstream myfile("16.2.input");
  std::string line;
  std::regex ops("(\\d+) (\\d+) (\\d+) (\\d+)");
  std::array<int, 4> registers({0, 0, 0, 0});

  if (myfile.is_open()) {
    while (getline(myfile, line)) {
      Instruction op;
      std::smatch match;
      assert((std::regex_search(line, match, ops)));
      op.opcode = codeMap[std::stoi(match[1].str())];
      op.input1 = std::stoi(match[2].str());
      op.input2 = std::stoi(match[3].str());
      op.output = std::stoi(match[4].str());
      mutate(op, registers);
    }
  }

  std::cout << "Register 0 at end: " << registers[0] << "\n";
}

int main() {
  auto operations = readSequence();
  auto codeMap = partOne(operations);
  partTwo(codeMap);
}