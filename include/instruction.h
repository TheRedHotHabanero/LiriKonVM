#ifndef INCLUDE_INSTRUCTION_H
#define INCLUDE_INSTRUCTION_H

#include "vm.h"

struct Instruction final {
    interpreter::OpCode opcode;
    interpreter::RegID reg_id;
    interpreter::Imm imm;
    interpreter::RegID GetSecondReg() {
        return imm >> 8;
    }
};

#endif // INCLUDE_INSTRUCTION_H