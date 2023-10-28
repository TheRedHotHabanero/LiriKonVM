#ifndef INCLUDE_VM_H
#define INCLUDE_VM_H

#include <cstdint>
namespace vm_numbers {
    constexpr uint64_t OPCODE_NUM = 30;
    constexpr uint64_t REG_NUM = 9;
}
namespace interpreter {

using Byte = uint8_t;
using IReg = int8_t;
using FReg = double;
using Instr = uint32_t;

using InstrID = uint32_t;
using RegID = uint8_t;
using OpCode = uint8_t;
using Imm = uint16_t;

}  // namespace interpreter

#endif  // INCLUDE_VM_HPP