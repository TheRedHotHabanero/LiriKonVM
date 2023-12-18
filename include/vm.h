#ifndef INCLUDE_VM_H
#define INCLUDE_VM_H

#include <cstdint>

namespace vm_numbers {

static constexpr uint64_t VM_DEFAULT_ARENA = 1 << 23; // 10 MB
static constexpr uint64_t VM_DIGIT_NUM = 10;
static constexpr uint64_t OPCODE_NUM = 31;
static constexpr uint64_t REG_NUM = 9;

} // namespace vm_numbers
namespace interpreter {

using Byte = uint8_t;
using IReg = int;
using FReg = double;
using Instr = uint32_t;

using InstrID = uint32_t;
using RegID = uint8_t;
using OpCode = uint8_t;
using Imm = uint16_t;

} // namespace interpreter

#endif // INCLUDE_VM_HPP