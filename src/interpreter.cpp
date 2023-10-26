#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../include/interpreter.h"
#include "../include/vm.h"
#include "../include/decoder.h"
#include "../include/runner.h"
#include "../isa/instructions.h"

Interpreter::Interpreter() {
    decoder_ = new Decoder();
    runner_ = new Runner();
}

Interpreter::~Interpreter() {
    ~Decoder();
    ~Runner();
}

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
        // while (iss >> word) {
        //     if (instructions_map.find(word) != instructions_map.end()) {
        //         program.push_back(
        //             static_cast<uint64_t>(instructions_map[word]));
        //         // std::cout << word << std::endl;
        //     }
        //     if (cells_map.find(word) != cells_map.end()) {
        //     }
        // }
    }

    file.close();
}

interpreter::Instr Interpreter::executeInstruction(interpreter::Byte *bytecode, interpreter::IReg pc) {
    return *reinterpret_cast<interpreter::Instr *>(&bytecode[pc]);
}

void Interpreter::executeProgram(interpreter::Byte *bytecode) {
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
    auto &registers = runner_->GetIRegs();
    auto &fregisters = runner_->GetFRegs();
    interpreter::IReg &pc = registers[9];
    Instruction *cur_instr = new Instruction;
    *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));
    goto *dispatch_table[cur_instr->GetSecondReg()];

    #define NEXT()                                          \
        pc += 4;                                            \
        *cur_instr = decoder_->decodeInstruction(executeInstruction(code, pc)); \
        goto *dispatch_table[cur_instr->GetSecondReg()];

HANDLE_ADD:
    accumulator += registers[operand];
    NEXT();
HANDLE_SUB:
    accumulator -= registers[operand];
    NEXT();
HANDLE_MUL:
    accumulator *= registers[operand];
    NEXT();
HANDLE_DIV:
    accumulator /= registers[operand];
    NEXT();
HANDLE_AND:
    accumulator &= registers[operand];
    NEXT();
HANDLE_OR:
    accumulator |= registers[operand];
    NEXT();
HANDLE_XOR:
    accumulator ^= registers[operand];
    NEXT();
HANDLE_SHL:
    accumulator <<= registers[operand];
    NEXT();
HANDLE_SHR:
    accumulator >>= registers[operand];
    NEXT();
HANDLE_ASHR:
    uint64_t shift_ashr = registers[operand];
    uint64_t sign = (accumulator < 0) ? -1 : 1;
    accumulator = sign * (abs(accumulator) >> shift_ashr);
    NEXT();
HANDLE_ASHL:
    uint64_t shift_ashl = registers[operand];
    accumulator = accumulator << shift_ashl;
    NEXT();
HANDLE_NEG:
    accumulator = -accumulator;
    NEXT();
HANDLE_MOV_IMM_TO_ACC:
    registers[operand] = operand;
    NEXT();
HANDLE_MOV_REG_TO_REG:
    accumulator = registers[operand];
    NEXT();
HANDLE_INPUT:
    uint64_t input;
    std::cin >> input;
    registers[operand] = input;
    NEXT();
HANDLE_OUTPUT:
    std::cout << accumulator << std::endl;
    NEXT();
HANDLE_RETURN:
    return;
HANDLE_SIN:
    accumulator = std::sin(accumulator);
   NEXT();
HANDLE_COS:
    accumulator = std::cos(accumulator);
    NEXT();
HANDLE_SQRT:
    accumulator = std::sqrt(accumulator);
   NEXT();
HANDLE_INVALID:
    std::cerr << "Error: Unknown opcode " << program[pc] << std::endl;
    exit(1);
}
