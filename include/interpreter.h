#ifndef INCLUDE_INTERPRETER_H
#define INCLUDE_INTERPRETER_H

#include <fstream>
#include <iostream>
#include <vector>
#include "decoder.h"
#include "runner.h"

class Interpreter final {
  public:
    Interpreter();
    void loadProgram(const std::string &filename);
    interpreter::Instr executeInstruction(interpreter::Byte *bytecode, interpreter::IReg pc);
    void executeProgram(interpreter::Byte *bytecode);
    std::vector<interpreter::Instr> GetProgram();
    ~Interpreter();

  private:
    std::vector<interpreter::Instr> program_;
    Decoder *decoder_;
    Runner *runner_;
};

#endif // INCLUDE_INTERPRETER_H