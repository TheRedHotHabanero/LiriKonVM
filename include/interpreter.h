#ifndef INCLUDE_INTERPRETER_H
#define INCLUDE_INTERPRETER_H

#include <fstream>
#include <vector>

#include "allocator.h"
#include "decoder.h"
#include "parser.h"
#include "runner.h"

class Interpreter final {
public:
  Interpreter();
  void loadProgram(const std::string filename);
  interpreter::Instr executeInstruction(interpreter::Byte *bytecode,
                                        interpreter::IReg pc);
  void executeProgram(interpreter::Byte *bytecode);
  std::vector<interpreter::Instr> GetProgram();
  ~Interpreter();

  auto getAllocator() {
    return *allocator_;
  }

private:
  Decoder *decoder_;
  Runner *runner_;
  Parser *parser_;
  std::unique_ptr<ArenaAllocator> allocator_;
};

#endif // INCLUDE_INTERPRETER_H