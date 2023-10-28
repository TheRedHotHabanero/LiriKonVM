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
    interpreter::Instr parse_4(interpreter::Instr opcode, interpreter::Instr dest, interpreter::Instr source_1, interpreter::Instr source_2);
    interpreter::Instr parse_3(interpreter::Instr opcode, interpreter::Instr dest, interpreter::Instr source);
    interpreter::Instr parse_2(interpreter::Instr opcode, interpreter::Instr dest);
    interpreter::Instr parse_1(interpreter::Instr opcode);
    void loadProgram(const std::string &filename);
    interpreter::Instr executeInstruction(interpreter::Byte *bytecode, interpreter::IReg pc);
    void executeProgram(interpreter::Byte *bytecode);
    std::vector<interpreter::Instr> GetProgram();
    ~Interpreter();

  private:
    std::vector<interpreter::Instr> words_;
    std::vector<interpreter::Instr> program_;
    Decoder *decoder_;
    Runner *runner_;
};

#endif // INCLUDE_INTERPRETER_H