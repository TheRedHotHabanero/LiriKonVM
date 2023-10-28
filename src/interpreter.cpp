#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "../include/interpreter.h"
#include "../isa/instructions.h"
#include "../include/vm.h"
#include "../include/decoder.h"
#include "../include/runner.h"

std::vector<interpreter::Instr> Interpreter::GetProgram() {
  return program_;
}

Interpreter::Interpreter() {
    decoder_ = new Decoder();
    runner_ = new Runner();
}

Interpreter::~Interpreter() {
    delete decoder_;
    delete runner_;
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
        while (iss >> word) {
            if (instructions_map.find(word) != instructions_map.end()) {
                program_.push_back(
                    static_cast<interpreter::Instr>(instructions_map[word]));
                continue;
            }
            if (cells_map.find(word) != cells_map.end()) {
                program_.push_back(
                    static_cast<interpreter::Instr>(cells_map[word]));
                continue;
            }
            if (f_cells_map.find(word) != f_cells_map.end()) {
                program_.push_back(
                    static_cast<interpreter::Instr>(cells_map[word]));
                continue;
            }
        }
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
                                     &&HANDLE_NEG,
                                     &&HANDLE_MOV_IMM_TO_ACC,
                                     &&HANDLE_MOV_REG_TO_REG,
                                     &&HANDLE_INPUT,
                                     &&HANDLE_OUTPUT,
                                     &&HANDLE_RETURN,
                                     &&HANDLE_SIN,
                                     &&HANDLE_COS,
                                     &&HANDLE_SQRT,
                                     &&HANDLE_POW,
                                     &&HANDLE_INVALID};
    auto &iregisters = runner_->GetIRegs();
    auto &fregisters = runner_->GetFRegs();
    interpreter::IReg &pc = iregisters[9];
    Instruction *cur_instr = new Instruction;
    *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));
    goto *dispatch_table[cur_instr->GetSecondReg()];

    #define NEXT()                                                                  \
        pc += 4;                                                                    \
        *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc)); \
        goto *dispatch_table[cur_instr->GetSecondReg()];                            \

HANDLE_ADD:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] + iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SUB:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] - iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MUL:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] * iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_DIV:
    // TODO: add case with zero div
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] / iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_AND:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] & iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_OR:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] | iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_XOR:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] ^ iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SHL:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] << iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SHR:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] >> iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_NEG:
    iregisters[IRegisters::ACC] = -iregisters[cur_instr->reg_id];
    NEXT();
HANDLE_MOV_IMM_TO_ACC:
    iregisters[IRegisters::ACC] = cur_instr->imm;
    NEXT();
HANDLE_MOV_REG_TO_REG:
    iregisters[cur_instr->reg_id] = iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_INPUT:
    std::cin >> iregisters[IRegisters::ACC];
    NEXT();
HANDLE_OUTPUT:
    std::cout << iregisters[IRegisters::ACC];
    NEXT();
HANDLE_RETURN:
    delete cur_instr;
    return;
HANDLE_SIN:
    fregisters[FRegisters::FACC] = std::sin(fregisters[cur_instr->reg_id]);
   NEXT();
HANDLE_COS:
    fregisters[FRegisters::FACC] = std::cos(fregisters[cur_instr->reg_id]);
    NEXT();
HANDLE_SQRT:
    fregisters[FRegisters::FACC] = std::sqrt(fregisters[cur_instr->reg_id]);
    NEXT();
HANDLE_POW:
    iregisters[FRegisters::FACC] = std::pow(fregisters[cur_instr->reg_id], cur_instr->imm);
    NEXT();
HANDLE_INVALID:
    std::cerr << "Error: Unknown opcode " << std::endl;
    exit(1);
}
