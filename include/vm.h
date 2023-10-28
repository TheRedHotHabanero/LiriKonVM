#ifndef INCLUDE_VM_H
#define INCLUDE_VM_H

#include <cstdint>
namespace interpreter {

using Byte = uint64_t;
using IReg = int64_t;
using FReg = double;
using Instr = uint64_t;

using InstrID = uint64_t;
using TypeID = uint64_t;
using RegID = uint64_t;
using OpCode = uint64_t;
using Imm = uint16_t;

}  // namespace interpreter

#endif  // INCLUDE_VM_HPP