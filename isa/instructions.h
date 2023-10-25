#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <unordered_map>

enum class OpCode: uint64_t {
    // Arithmetical
    ADD,
    SUB,
    MUL,
    DIV,

    // Logical
    AND,
    OR,
    XOR,
    NEG,

    // Bitwise
    SHL,
    SHR,
    ASHR,
    ASHL,

    // Memory
    MOV_IMM_TO_ACC,
    MOV_REG_TO_REG,

    // Math + IO
    INPUT,
    OUTPUT,
    RETURN,
    SIN,
    COS,
    SQRT,


    INVALID,
};

enum class IRegisters: uint64_t {
    ACC = 0x0,
    R0 = 0x1,
    R1 = 0x2,
    R2 = 0x3,
    R3 = 0x4,
    R4 = 0x5,
    R5 = 0x6,
    R6 = 0x7,
    R7 = 0x8,
};

enum class FRegisters: uint64_t {
    FACC = 0x0,
    FR0 = 0x1,
    FR1 = 0x2,
    FR2 = 0x3,
    FR3 = 0x4,
    FR4 = 0x5,
    FR5 = 0x6,
    FR6 = 0x7,
    FR7 = 0x8,
};

std::unordered_map<std::string, OpCode> instructions_map {
    {"ADD", OpCode::ADD},
    {"SUB", OpCode::SUB},
    {"MUL", OpCode::MUL},
    {"DIV", OpCode::DIV},
    {"AND", OpCode::AND},
    {"OR", OpCode::OR},
    {"XOR", OpCode::XOR},
    {"SHL", OpCode::SHL},
    {"SHR", OpCode::SHR},
    {"ASHR", OpCode::ASHR},
    {"ASHL", OpCode::ASHL},
    {"NEG", OpCode::NEG},
    {"MOV_IMM_TO_ACC", OpCode::MOV_IMM_TO_ACC},
    {"MOV_REG_TO_REG", OpCode::MOV_REG_TO_REG},
    {"INPUT", OpCode::INPUT},
    {"OUTPUT", OpCode::OUTPUT},
    {"RETURN", OpCode::RETURN},
    {"SIN", OpCode::SIN},
    {"COS", OpCode::COS},
    {"SQRT", OpCode::SQRT},
    {"INVALID", OpCode::INVALID},
};

std::unordered_map<std::string, IRegisters> cells_map {
    {"r0", IRegisters::R0},
    {"r1", IRegisters::R1},
    {"r2", IRegisters::R2},
    {"r3", IRegisters::R3},
    {"r4", IRegisters::R4},
    {"r5", IRegisters::R5},
    {"r6", IRegisters::R6},
    {"r7", IRegisters::R7},
    {"ACC", IRegisters::ACC},
};

#endif // INSTRUCTIONS_H