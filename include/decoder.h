#ifndef INCLUDE_DECODER_H
#define INCLUDE_DECODER_H

#include "instruction.h"
#include "vm.h"
#include <iostream>

struct Decoder final {
  Decoder() = default;
  ~Decoder() = default;
  Instruction decodeInstruction(interpreter::Instr intr_to_decode);
  static Decoder *Init();
};

#endif // INCLUDE_DECODER_HPP
