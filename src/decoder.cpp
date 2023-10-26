#include "../include/decoder.h"
#include "../isa/instructions.h"
#include "../include/vm.h"

Instruction Decoder::decodeInstruction(interpreter::Instr intr_to_decode)
{
    OpCode opcode = static_cast<OpCode>(intr_to_decode & (1ULL << 8 - 1));
    interpreter::RegID reg_id = ((intr_to_decode >> 8) & (1ULL << 8 - 1));
    uint64_t upper_value = ((intr_to_decode >> 16) & (1ULL << 8 - 1));
    uint64_t lower_value = (intr_to_decode >> 24);
    interpreter::Imm imm = (upper_value << 8) + lower_value;
    return {opcode, reg_id, imm};
}