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
    decoder_ = Decoder::Init();
    runner_ = Runner::Init();
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
        int counter = 0;
        while (iss >> word) {
            if (instructions_map.find(word) != instructions_map.end()) {
                words_.push_back(
                    static_cast<interpreter::Instr>(instructions_map[word]));
                counter += 1;
            }
            if (cells_map.find(word) != cells_map.end()) {
                words_.push_back(
                    static_cast<interpreter::Instr>(cells_map[word]));
                counter += 1;
            }
            if (f_cells_map.find(word) != f_cells_map.end()) {
                words_.push_back(
                    static_cast<interpreter::Instr>(cells_map[word]));
                counter += 1;
            }
        }
        if (counter == 4) {
            program_.push_back(parse_4(words_[words_.size() - 4], words_[words_.size()- 3], words_[words_.size() - 2], words_[words_.size() - 1]));
        } else if (counter == 3) {
            program_.push_back(parse_3(words_[words_.size()- 3], words_[words_.size() - 2], words_[words_.size() - 1]));
        } else if (counter == 2) {
            program_.push_back(parse_2(words_[words_.size() - 2], words_[words_.size() - 1]));
        } else if (counter == 1) {
            program_.push_back(parse_1(words_[words_.size() - 1]));
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
                                     &&HANDLE_ADDF,
                                     &&HANDLE_SUBF,
                                     &&HANDLE_MULF,
                                     &&HANDLE_DIVF,
                                     &&HANDLE_AND,
                                     &&HANDLE_OR,
                                     &&HANDLE_XOR,
                                     &&HANDLE_SHL,
                                     &&HANDLE_SHR,
                                     &&HANDLE_NEG,
                                     &&HANDLE_NEGF,
                                     &&HANDLE_MOV_IMM_TO_ACC,
                                     &&HANDLE_MOV_REG_TO_REG,
                                     &&HANDLE_MOV_IMM_TO_ACCF,
                                     &&HANDLE_MOV_REG_TO_REGF,
                                     &&HANDLE_INPUT,
                                     &&HANDLE_OUTPUT,
                                     &&HANDLE_INPUTF,
                                     &&HANDLE_OUTPUTF,
                                     &&HANDLE_RETURN,
                                     &&HANDLE_SIN,
                                     &&HANDLE_COS,
                                     &&HANDLE_SQRT,
                                     &&HANDLE_POW,
                                     &&HANDLE_INVALID};
    auto &iregisters = runner_->GetIRegs();
    auto &fregisters = runner_->GetFRegs();
    interpreter::IReg &pc = iregisters[vm_numbers::REG_NUM - 1];
    Instruction *cur_instr = new Instruction();
    std::cout << "execute 1" << std::endl;
    *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));
    std::cout << "execute 2" << std::endl;
    goto *dispatch_table[cur_instr->GetInstOpcode()];

    #define NEXT()                                                                  \
        pc += 4;                                                                    \
        *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc)); \
        goto *dispatch_table[cur_instr->GetInstOpcode()];                           \

HANDLE_ADD:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] + iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_ADDF:
    fregisters[FRegisters::FACC] = fregisters[cur_instr->reg_id] + fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SUB:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] - iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SUBF:
    fregisters[FRegisters::FACC] = fregisters[cur_instr->reg_id] - fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MUL:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] * iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MULF:
    fregisters[FRegisters::FACC] = fregisters[cur_instr->reg_id] * fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_DIV:
    // TODO: add case with zero div
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id] / iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_DIVF:
    // TODO: add case with zero div
    fregisters[FRegisters::FACC] = fregisters[cur_instr->reg_id] / fregisters[cur_instr->GetSecondReg()];
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
HANDLE_NEGF:
    fregisters[FRegisters::FACC] = -fregisters[cur_instr->reg_id];
    NEXT();
HANDLE_MOV_IMM_TO_ACC:
    iregisters[IRegisters::ACC] = cur_instr->imm;
    NEXT();
HANDLE_MOV_IMM_TO_ACCF:
    fregisters[FRegisters::FACC] = cur_instr->imm;
    NEXT();
HANDLE_MOV_REG_TO_REG:
    iregisters[cur_instr->reg_id] = iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MOV_REG_TO_REGF:
    fregisters[cur_instr->reg_id] = fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_INPUT:
    std::cin >> iregisters[IRegisters::ACC];
    NEXT();
HANDLE_INPUTF:
    std::cin >> fregisters[FRegisters::FACC];
    NEXT();
HANDLE_OUTPUT:
    std::cout << iregisters[IRegisters::ACC];
    NEXT();
HANDLE_OUTPUTF:
    std::cout << fregisters[FRegisters::FACC];
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

interpreter::Instr parse_4(interpreter::Instr opcode, interpreter::Instr dest, interpreter::Instr source_1, interpreter::Instr source_2) {

    interpreter::Instr value = 0;
    interpreter::RegID reg_id = dest;
    interpreter::Imm imm = 
    switch (opcode)
    {
        case OpCode::ADD:
        case OpCode::DIV:
        case OpCode::SUB:
        case OpCode::MUL:
        case OpCode::AND:
        case OpCode::OR:
        case OpCode::XOR:
        case OpCode::NEG: 
            std::cout << "hehe";
        default:
            value |= (reg_id << 8);
            value |= (imm << 16);
            break;
    }
}
interpreter::Instr parse_3(interpreter::Instr opcode, interpreter::Instr dest, interpreter::Instr source) {};
interpreter::Instr parse_2(interpreter::Instr opcode, interpreter::Instr dest) {};
interpreter::Instr parse_1(interpreter::Instr opcode) {};