#ifndef INCLUDE_INTERPRETER_H
#define INCLUDE_INTERPRETER_H

#include <fstream>
#include <iostream>
#include <vector>
#include "decoder.h"
#include "runner.h"
#include "vm.h"

class Interpreter final {
  public:
    Interpreter();
    void loadProgram(const std::string &filename);
    interpreter::Instr executeInstruction(interpreter::Byte *bytecode, interpreter::IReg pc);
    void executeProgram(interpreter::Byte *bytecode);
    ~Interpreter();

  private:
    Decoder *decoder_;
    Runner *runner_;
};

#endif // INCLUDE_INTERPRETER_H