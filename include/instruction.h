#ifndef INCLUDE_INSTRUCTION_H
#define INCLUDE_INSTRUCTION_H

#include "vm.h"

struct Instruction final {
    interpreter::RegID reg_id;
    interpreter::Imm imm;
    interpreter::OpCode opcode_;

    interpreter::RegID GetSecondReg() {
        return imm >> 8;
    }
    interpreter::OpCode GetInstOpcode() {
        return opcode_;
    }
};

#endif // INCLUDE_INSTRUCTION_H