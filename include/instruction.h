#ifndef INCLUDE_INSTRUCTION_H
#define INCLUDE_INSTRUCTION_H

#include "../isa/instructions.h"
#include "vm.h"

struct Instruction final {
    OpCode opcode;
    // std::vector<uint64_t> operands;
    interpreter::RegID reg_id;
    interpreter::Imm imm;
    interpreter::RegID GetSecondReg() {
        return imm >> 8;
    }
};

#endif // INCLUDE_INSTRUCTION_H