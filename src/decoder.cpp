#include "../include/decoder.h"
#include <iostream>

Decoder *Decoder::Init() {
    return new Decoder;
}

Instruction Decoder::decodeInstruction(interpreter::Instr intr_to_decode) {
    std::cout << "got 1" << std::endl;
    interpreter::OpCode opcode = static_cast<interpreter::OpCode>(intr_to_decode & (1ULL << 8 - 1));
    std::cout << "got 2" << std::endl;
    interpreter::RegID reg_id = ((intr_to_decode >> 8) & (1ULL << 8 - 1));
    uint64_t upper_value = ((intr_to_decode >> 16) & (1ULL << 8 - 1));
    uint64_t lower_value = (intr_to_decode >> 24);
    interpreter::Imm imm = (upper_value << 8) + lower_value;
    return {opcode, reg_id, imm};
}