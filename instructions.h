#include <iostream>

// Define bytecode instructions
// TODO: add different types
enum class OpCode {
    INVALID,
    ADD, // acc = (acc + vs) % 2^32
    SUB, // acc = (acc - vs) % 2^32
    MUL, // acc = (acc * vs) % 2^32
    DIV, //  if acc == INT32_MIN and vs == -1 then acc = INT32_MIN , else: acc = acc / vs
    AND, // acc = acc & vs
    OR, // acc = acc | vs
    XOR, // acc = vs1 ^ vs2
    SHL, // acc = (vs1 << (vs2 & 0x1f)) % 2^32
    SHR, // acc = vs1 >> (vs2 & 0x1f)
    ASHR, // acc = (vs1 < 0) ? ~(~vs1 >> vs2 & 0x1f) : vs1 >> vs2 & 0x1f
    ASHL,
    NEG, // acc = -acc
    MOV_IMM_TO_REG, // move integer immediate into a register.
    MOV_REG_TO_ACC,
    INPUT, // IO intrinsic - input
    OUTPUT, // // IO intrinsic - output
    HALT,
    SIN,
    COS,
    SQRT,
};

enum class Cells {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    IMM,
    ACC,
};