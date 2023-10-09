// Define bytecode instructions
// TODO: add different types
#include <unordered_map>
enum class OpCode: uint64_t {
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
    ASHL, // same to the left
    NEG, // acc = -acc
    MOV_IMM_TO_REG, // move immediate into a register.
    MOV_REG_TO_ACC,
    INPUT, // IO intrinsic - input
    OUTPUT, // // IO intrinsic - output
    HALT, // return
    SIN, // help with math functions :)
    COS, // help with math functions :)
    SQRT, // help with math functions :)
    INVALID,
};

enum class Cells: uint64_t {
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
    {"MOV_IMM_TO_REG", OpCode::MOV_IMM_TO_REG},
    {"MOV_REG_TO_ACC", OpCode::MOV_REG_TO_ACC},
    {"INPUT", OpCode::INPUT},
    {"OUTPUT", OpCode::OUTPUT},
    {"HALT", OpCode::HALT},
    {"SIN", OpCode::SIN},
    {"COS", OpCode::COS},
    {"SQRT", OpCode::SQRT},
    {"INVALID", OpCode::INVALID},
};

std::unordered_map<std::string, Cells> cells_map {
    {"r0", Cells::R0},
    {"r1", Cells::R1},
    {"r2", Cells::R2},
    {"r3", Cells::R3},
    {"r4", Cells::R4},
    {"r5", Cells::R5},
    {"r6", Cells::R6},
    {"r7", Cells::R7},
    {"IMM", Cells::IMM},
    {"ACC", Cells::ACC},
};