#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <fstream>
#include <iostream>
#include <vector>

class Interpreter final {
  public:
    Interpreter();
    void loadProgram(const std::string &filename);
    void executeProgram();
    ~Interpreter();

  private:
    std::vector<uint64_t> registers;
    std::vector<uint64_t> program;
    int accumulator;
    const int regNum = 8;

    void executeInstruction(uint64_t &pc);
};

#endif // INTERPRETER_H
