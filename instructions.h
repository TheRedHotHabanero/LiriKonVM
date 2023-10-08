#include <iostream>

// Define bytecode instructions
enum class OpCode {
    ADD, // acc = (acc + vs) % 2^32
    SUB, // acc = (acc - vs) % 2^32
    MUL, // acc = (acc * vs) % 2^32
    AND, // acc = acc & vs
    OR, // acc = acc | vs
    XOR, // acc = vs1 ^ vs2
    SHL, // acc = (vs1 << (vs2 & 0x1f)) % 2^32
    SHR, // acc = vs1 >> (vs2 & 0x1f)
    ASHR, // acc = (vs1 < 0) ? ~(~vs1 >> vs2 & 0x1f) : vs1 >> vs2 & 0x1f
    ASHL,
    DIV, //  if acc == INT32_MIN and vs == -1 then acc = INT32_MIN , else: acc = acc / vs
    NEG, // acc = -acc
    MOV_IMM_TO_REG, // move integer immediate into a register.
    MOV_REG_TO_ACC,
    INTRINSIC_IO,
    HALT,

    PUSH_CONST,
    SIN,
    COS,
    SQRT,
    PRINT,
    INTRINSIC_INPUT,
    INTRINSIC_OUTPUT,
};