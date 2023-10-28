#include "../include/decoder.h"
#include <iostream>

Decoder *Decoder::Init() {
    return new Decoder;
}

Instruction Decoder::decodeInstruction(interpreter::Instr intr_to_decode) {
    interpreter::OpCode opcode = static_cast<interpreter::OpCode>(intr_to_decode & ((1UL << 8) - 1));
    std::cout << "opcode=" << static_cast<int>(opcode) << std::endl;
    interpreter::RegID reg_id = ((intr_to_decode >> 8) & ((1UL << 8) - 1));
    uint16_t upper_value = ((intr_to_decode >> 16) & ((1UL << 8) - 1));
    uint16_t lower_value = (intr_to_decode >> 24);
    interpreter::Imm imm = (upper_value << 8) + lower_value;
    return {opcode, reg_id, imm};
}