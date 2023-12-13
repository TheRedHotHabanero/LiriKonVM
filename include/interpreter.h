#ifndef INCLUDE_INTERPRETER_H
#define INCLUDE_INTERPRETER_H

#include "parser.h"
#include "decoder.h"
#include "runner.h"
#include "allocator.h"
#include <fstream>
#include <iostream>
#include <vector>

class Interpreter final {
  public:
    Interpreter();
    void loadProgram(const std::string &filename);
    interpreter::Instr executeInstruction(interpreter::Byte *bytecode,
                                          interpreter::IReg pc);
    void executeProgram(interpreter::Byte *bytecode);
    std::vector<interpreter::Instr> GetProgram();
    ~Interpreter();

  private:
    Decoder *decoder_;
    Runner *runner_;
    Parser *parser_;
    std::unique_ptr<ArenaAllocator> allocator_;
};

#endif // INCLUDE_INTERPRETER_H