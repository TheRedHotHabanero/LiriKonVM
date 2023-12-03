#ifndef INCLUDE_INTERPRETER_H
#define INCLUDE_INTERPRETER_H

#include "decoder.h"
#include "runner.h"
#include <fstream>
#include <iostream>
#include <vector>

class Interpreter final {
  public:
    Interpreter();
    interpreter::Instr parse_3(uint8_t opcode, uint32_t source_1,
                               uint32_t source_2);
    interpreter::Instr parse_2(uint8_t opcode, uint32_t source);
    interpreter::Instr parse_1(uint8_t opcode);
    void loadProgram(const std::string &filename);
    interpreter::Instr executeInstruction(interpreter::Byte *bytecode,
                                          interpreter::IReg pc);
    void executeProgram(interpreter::Byte *bytecode);
    std::vector<interpreter::Instr> GetProgram();
    ~Interpreter();

  private:
    std::vector<uint8_t> words_ = {};
    std::vector<interpreter::Instr> program_ = {};
    Decoder *decoder_;
    Runner *runner_;
};

#endif // INCLUDE_INTERPRETER_H