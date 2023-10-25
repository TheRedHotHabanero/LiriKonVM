#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../include/interpreter.h"
#include "../isa/instructions.h"
#include "../include/vm.h"

Interpreter::Interpreter() {};

Interpreter::~Interpreter() {}

// Декодер инструкции
//Instruction decodeInstruction(const std::vector<uint64_t> &bytecode,
//                              size_t &offset) {
//    Instruction instruction;
//
//    // Читаем opcode (первый байт инструкции)
//    instruction.opcode = static_cast<OpCode>(bytecode[offset]);
//    offset++;
//
//    switch (instruction.opcode) {
//    case OpCode::ADD:
//    case OpCode::SUB:
//    case OpCode::MUL:
//    case OpCode::DIV:
//    case OpCode::AND:
//    case OpCode::OR:
//    case OpCode::XOR:
//    case OpCode::SHL:
//    case OpCode::SHR:
//    case OpCode::ASHR:
//    case OpCode::ASHL:
//        instruction.operands.push_back(bytecode[offset]);
//        offset++;
//        instruction.operands.push_back(bytecode[offset + 1]);
//        // offset++;
//        instruction.operands.push_back(bytecode[offset + 2]);
//        // offset++;
//    case OpCode::MOV_IMM_TO_ACC:
//    case OpCode::MOV_REG_TO_REG:
//        instruction.operands.push_back(bytecode[offset]);
//        offset++;
//        break;
//    case OpCode::INPUT:
//    case OpCode::OUTPUT:
//    case OpCode::SIN:
//    case OpCode::COS:
//    case OpCode::SQRT:
//    case OpCode::RETURN:
//        break;
//    case OpCode::NEG:
//        instruction.operands.push_back(bytecode[offset]);
//        offset++;
//        break;
//    default:
//        std::cerr << "Error: Unknown opcode "
//                  << static_cast<int>(instruction.opcode) << std::endl;
//        break;
//    }
//
//    for (auto &it : instruction.operands) {
//        std::cout << instruction.operands[it] << std::endl;
//    }
//
//    std::cout << "---" << std::endl;
//
//    return instruction;
//}

void Interpreter::loadProgram(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the program file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            if (instructions_map.find(word) != instructions_map.end()) {
                program.push_back(
                    static_cast<uint64_t>(instructions_map[word]));
                // std::cout << word << std::endl;
            }
            if (cells_map.find(word) != cells_map.end()) {
            }
        }
    }

    file.close();
}

void Interpreter::executeProgram([[maybe_unused]] Byte *bytecode) {
    // uint64_t curr_inst = 0;
    // uint64_t pc = 0;
    // uint64_t size = program.size();
    // while (curr_inst < size) {
    //     executeInstruction(pc);
    //     curr_inst += 1;
    // }
}

int getRegisterIndex(const std::string &registerName) {
    if (registerName[0] == 'r' && registerName.size() > 1) {
        return std::stoi(registerName.substr(1));
    }
    return 0;
}

// void Interpreter::executeInstruction(uint64_t &pc) {

Instr executeInstruction([[maybe_unused]] Byte *bytecode, interpreter::Reg pc);
    static void *dispatch_table[] = {&&HANDLE_ADD,
                                     &&HANDLE_SUB,
                                     &&HANDLE_MUL,
                                     &&HANDLE_DIV,
                                     &&HANDLE_AND,
                                     &&HANDLE_OR,
                                     &&HANDLE_XOR,
                                     &&HANDLE_SHL,
                                     &&HANDLE_SHR,
                                     &&HANDLE_ASHR,
                                     &&HANDLE_ASHL,
                                     &&HANDLE_NEG,
                                     &&HANDLE_MOV_IMM_TO_ACC,
                                     &&HANDLE_MOV_REG_TO_REG,
                                     &&HANDLE_INPUT,
                                     &&HANDLE_OUTPUT,
                                     &&HANDLE_RETURN,
                                     &&HANDLE_SIN,
                                     &&HANDLE_COS,
                                     &&HANDLE_SQRT,
                                     &&HANDLE_INVALID};

    uint64_t operand = program[pc];
    goto *dispatch_table[operand];

HANDLE_ADD:
    accumulator += registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_SUB:
    accumulator -= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_MUL:
    accumulator *= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_DIV:
    accumulator /= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_AND:
    accumulator &= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_OR:
    accumulator |= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_XOR:
    accumulator ^= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_SHL:
    accumulator <<= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_SHR:
    accumulator >>= registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_ASHR:
    uint64_t shift_ashr = registers[operand];
    uint64_t sign = (accumulator < 0) ? -1 : 1;
    accumulator = sign * (abs(accumulator) >> shift_ashr);
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_ASHL:
    uint64_t shift_ashl = registers[operand];
    accumulator = accumulator << shift_ashl;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_NEG:
    accumulator = -accumulator;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_MOV_IMM_TO_ACC:
    registers[operand] = operand;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_MOV_REG_TO_REG:
    accumulator = registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_INPUT:
    uint64_t input;
    std::cin >> input;
    registers[operand] = input;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_OUTPUT:
    std::cout << accumulator << std::endl;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_RETURN:
    return;
HANDLE_SIN:
    accumulator = std::sin(accumulator);
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_COS:
    accumulator = std::cos(accumulator);
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_SQRT:
    accumulator = std::sqrt(accumulator);
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_INVALID:
    std::cerr << "Error: Unknown opcode " << program[pc] << std::endl;
    exit(1);
}
