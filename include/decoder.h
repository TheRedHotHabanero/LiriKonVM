#ifndef INCLUDE_DECODER_H
#define INCLUDE_DECODER_H

#include "instruction.h"
#include "vm.h"

class Decoder final {
public:
    Decoder() = default;
    ~Decoder() = default;
    Instruction decodeInstruction(interpreter::Instr intr_to_decode);
};

#endif  // INCLUDE_DECODER_HPP