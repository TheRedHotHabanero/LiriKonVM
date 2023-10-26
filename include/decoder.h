#ifndef INCLUDE_DECODER_H
#define INCLUDE_DECODER_H

#include "instruction.h"
#include "vm.h"

struct Decoder final {
    Decoder();
    ~Decoder() {
        if (this == nullptr) {
            return;
        }
        delete this;
    };
    Instruction decodeInstruction(interpreter::Instr intr_to_decode);
};

#endif  // INCLUDE_DECODER_HPP