#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <unordered_map>
#include <iostream>

namespace vm_numbers {
    constexpr uint64_t OPCODE_NUM = 30;
    constexpr uint64_t REG_NUM = 9;
}

enum OpCode : uint64_t {
    // Arithmetical
    ADD,
    SUB,
    MUL,
    DIV,

    ADDF,
    SUBF,
    MULF,
    DIVF,

    // Logical
    AND,
    OR,
    XOR,
    NEG,
    NEGF,

    // Bitwise
    SHL, // not working now
    SHR, // not working now

    // Memory
    MOV_IMM_TO_ACC,
    MOV_REG_TO_REG,
    MOV_IMM_TO_ACCF,
    MOV_REG_TO_REGF,

    // Math + IO
    INPUT,
    OUTPUT,
    INPUTF,
    OUTPUTF,
    RETURN,
    SIN,
    COS,
    SQRT,
    POW,

    INVALID,
};

enum IRegisters : uint64_t {
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

enum FRegisters : uint64_t {
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

std::unordered_map<std::string, OpCode> instructions_map{
    {"ADD", OpCode::ADD},
    {"SUB", OpCode::SUB},
    {"MUL", OpCode::MUL},
    {"DIV", OpCode::DIV},
    {"ADDF", OpCode::ADDF},
    {"SUBF", OpCode::SUBF},
    {"MULF", OpCode::MULF},
    {"DIVF", OpCode::DIVF},
    {"AND", OpCode::AND},
    {"OR", OpCode::OR},
    {"XOR", OpCode::XOR},
    {"SHL", OpCode::SHL},
    {"SHR", OpCode::SHR},
    {"NEG", OpCode::NEG},
    {"NEGF", OpCode::NEGF},
    {"MOV_IMM_TO_ACC", OpCode::MOV_IMM_TO_ACC},
    {"MOV_REG_TO_REG", OpCode::MOV_REG_TO_REG},
    {"MOV_IMM_TO_ACCF", OpCode::MOV_IMM_TO_ACCF},
    {"MOV_REG_TO_REGF", OpCode::MOV_REG_TO_REGF},
    {"INPUT", OpCode::INPUT},
    {"OUTPUT", OpCode::OUTPUT},
    {"INPUTF", OpCode::INPUTF},
    {"OUTPUTF", OpCode::OUTPUTF},
    {"RETURN", OpCode::RETURN},
    {"SIN", OpCode::SIN},
    {"COS", OpCode::COS},
    {"SQRT", OpCode::SQRT},
    {"POW", OpCode::POW},
    {"INVALID", OpCode::INVALID},
};

std::unordered_map<std::string, IRegisters> cells_map {
    {"r0", IRegisters::R0}, {"r1", IRegisters::R1}, {"r2", IRegisters::R2},
    {"r3", IRegisters::R3}, {"r4", IRegisters::R4}, {"r5", IRegisters::R5},
    {"r6", IRegisters::R6}, {"r7", IRegisters::R7}, {"ACC", IRegisters::ACC},
};

std::unordered_map<std::string, FRegisters> f_cells_map {
    {"fr0", FRegisters::FR0}, {"fr1", FRegisters::FR1}, {"f2", FRegisters::FR2},
    {"fr3", FRegisters::FR3}, {"fr4", FRegisters::FR4}, {"f5", FRegisters::FR5},
    {"fr6", FRegisters::FR6}, {"fr7", FRegisters::FR7}, {"facc", FRegisters::FACC},
};

#endif // INSTRUCTIONS_H