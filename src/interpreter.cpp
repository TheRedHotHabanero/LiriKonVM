#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "../include/decoder.h"
#include "../include/interpreter.h"
#include "../include/runner.h"
#include "../include/vm.h"
#include "../isa/instructions.h"
#include "../include/allocator.h"

std::vector<interpreter::Instr> Interpreter::GetProgram() { return program_; }

Interpreter::Interpreter() {
    decoder_ = Decoder::Init();
    runner_ = Runner::Init();
    allocator_ = std::make_unique<ArenaAllocator>(ArenaAllocator::DEFAULT_ARENA);
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
            uint16_t num_arg = 0;
            if (instructions_map.find(word) != instructions_map.end()) {
                words_.push_back(instructions_map[word]);
                counter += 1;
            } else if (cells_map.find(word) != cells_map.end()) {
                words_.push_back(cells_map[word]);
                counter += 1;
            } else if (f_cells_map.find(word) != f_cells_map.end()) {
                words_.push_back(f_cells_map[word]);
                counter += 1;
            } else {
                try {
                    num_arg = std::stoi(word);
                    words_.push_back(num_arg);
                    counter += 1;
                } catch (std::invalid_argument const &ex) {
                }
            }
        };
        if (counter == 3) {
            program_.push_back(parse_3(words_[words_.size() - 3],
                                       words_[words_.size() - 2],
                                       words_[words_.size() - 1]));
        } else if (counter == 2) {
            program_.push_back(
                parse_2(words_[words_.size() - 2], words_[words_.size() - 1]));
        } else if (counter == 1) {
            program_.push_back(parse_1(words_[words_.size() - 1]));
        } else {
            std::cerr << "Error in creating instruction." << std::endl;
        }
    }

    file.close();
}

interpreter::Instr Interpreter::executeInstruction(interpreter::Byte *bytecode,
                                                   interpreter::IReg pc) {
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
                                     &&HANDLE_NEG,
                                     &&HANDLE_NEGF,
                                     &&HANDLE_SHL,
                                     &&HANDLE_SHR,
                                     &&HANDLE_MOV_IMM_TO_ACC,
                                     &&HANDLE_MOV_REG_TO_REG,
                                     &&HANDLE_MOV_IMM_TO_ACCF,
                                     &&HANDLE_MOV_REG_TO_REGF,
                                     &&HANDLE_MOV_ACC_TO_REG,
                                     &&HANDLE_MOV_ACC_TO_REGF,
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
    *cur_instr = decoder_->decodeInstruction(executeInstruction(bytecode, pc));
    goto *dispatch_table[cur_instr->GetInstOpcode()];

#define NEXT()                                                                 \
    pc += 4;                                                                   \
    *cur_instr =                                                               \
        decoder_->decodeInstruction(executeInstruction(bytecode, pc));         \
    goto *dispatch_table[cur_instr->GetInstOpcode()];

HANDLE_ADD:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] + iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_ADDF:
    fregisters[FRegisters::FACC] =
        fregisters[cur_instr->reg_id] + fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SUB:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] - iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SUBF:
    fregisters[FRegisters::FACC] =
        fregisters[cur_instr->reg_id] - fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MUL:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] * iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MULF:
    fregisters[FRegisters::FACC] =
        fregisters[cur_instr->reg_id] * fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_DIV:
    if (iregisters[cur_instr->GetSecondReg()] == 0) {
        std::cerr << "Error. Devision by zero." << std::endl;
        return;
    }
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] / iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_DIVF:
    if (fregisters[cur_instr->GetSecondReg()] == 0) {
        std::cerr << "Error. Devision by zero." << std::endl;
        return;
    }
    fregisters[FRegisters::FACC] =
        fregisters[cur_instr->reg_id] / fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_AND:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] & iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_OR:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] | iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_XOR:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] ^ iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SHL:
    iregisters[IRegisters::ACC] = iregisters[cur_instr->reg_id]
                                  << iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_SHR:
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] >> iregisters[cur_instr->GetSecondReg()];
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
    fregisters[FRegisters::FACC] = static_cast<double>(cur_instr->imm);
    NEXT();
HANDLE_MOV_ACC_TO_REG:
    iregisters[cur_instr->reg_id] = iregisters[IRegisters::ACC];
    NEXT();
HANDLE_MOV_ACC_TO_REGF:
    fregisters[cur_instr->reg_id] = fregisters[FRegisters::FACC];
    NEXT();
HANDLE_MOV_REG_TO_REG:
    iregisters[cur_instr->reg_id] = iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_MOV_REG_TO_REGF:
    fregisters[cur_instr->reg_id] = fregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_INPUT:
    std::cin >> iregisters[cur_instr->reg_id];
    NEXT();
HANDLE_INPUTF:
    std::cin >> fregisters[cur_instr->reg_id];
    NEXT();
HANDLE_OUTPUT:
    std::cout << "Output: " << iregisters[IRegisters::ACC] << std::endl;
    NEXT();
HANDLE_OUTPUTF:
    std::cout << "Output: " << fregisters[FRegisters::FACC] << std::endl;
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
    if (fregisters[cur_instr->reg_id] < 0) {
        std::cerr << "Error. Expression for sqrt is < 0." << std::endl;
        return;
    }
    fregisters[FRegisters::FACC] = std::sqrt(fregisters[cur_instr->reg_id]);
    NEXT();
HANDLE_POW:
    fregisters[FRegisters::FACC] = std::pow(
        fregisters[cur_instr->reg_id], fregisters[cur_instr->GetSecondReg()]);
    NEXT();
HANDLE_INVALID:
    std::cerr << "Error: Unknown opcode " << std::endl;
    exit(1);
}

interpreter::Instr Interpreter::parse_3(uint8_t opcode, uint32_t source_1,
                                        uint32_t source_2) {

    interpreter::Instr value = 0;

    switch (opcode) {
    case OpCode::ADD:
        value |= OpCode::ADD;
        break;
    case OpCode::DIV:
        value |= OpCode::DIV;
        break;
    case OpCode::SUB:
        value |= OpCode::SUB;
        break;
    case OpCode::MUL:
        value |= OpCode::MUL;
        break;
    case OpCode::AND:
        value |= OpCode::AND;
        break;
    case OpCode::OR:
        value |= OpCode::OR;
        break;
    case OpCode::XOR:
        value |= OpCode::XOR;
        break;
    case OpCode::NEG:
        value |= OpCode::NEG;
        break;
    case OpCode::MOV_REG_TO_REG:
        value |= OpCode::MOV_REG_TO_REG;
        break;
    // --------------------------------
    case OpCode::ADDF:
        value |= OpCode::ADDF;
        break;
    case OpCode::DIVF:
        value |= OpCode::DIVF;
        break;
    case OpCode::SUBF:
        value |= OpCode::SUBF;
        break;
    case OpCode::MULF:
        value |= OpCode::MULF;
        break;
    case OpCode::MOV_REG_TO_REGF:
        value |= OpCode::MOV_REG_TO_REGF;
        break;
    case OpCode::POW:
        value |= OpCode::POW;
        break;
    default:
        break;
    }
    value |= (source_1 << 8);
    value |= (source_2 << 16);
    return value;
}

interpreter::Instr Interpreter::parse_2(uint8_t opcode, uint32_t source) {
    interpreter::Instr value = 0;
    switch (opcode) {
    case OpCode::INPUT:
        value |= OpCode::INPUT;
        value |= (source << 8);
        break;
    case OpCode::INPUTF:
        value |= OpCode::INPUTF;
        value |= (source << 8);
        break;
    case OpCode::NEGF:
        value |= OpCode::NEGF;
        value |= (source << 8);
        break;
    case OpCode::SQRT:
        value |= OpCode::SQRT;
        value |= (source << 8);
        break;
    case OpCode::MOV_IMM_TO_ACC:
        value |= OpCode::MOV_IMM_TO_ACC;
        value |= ((source >> 8) & ((1UL << 8) - 1)) << 16;
        value |= (source & ((1UL << 8) - 1)) << 24;
        break;
    case OpCode::MOV_IMM_TO_ACCF:
        value |= OpCode::MOV_IMM_TO_ACCF;
        value |= ((source >> 8) & ((1UL << 8) - 1)) << 16;
        value |= (source & ((1UL << 8) - 1)) << 24;
        break;
    case OpCode::MOV_ACC_TO_REG:
        value |= OpCode::MOV_ACC_TO_REG;
        value |= (source << 8);
        break;
    case OpCode::MOV_ACC_TO_REGF:
        value |= OpCode::MOV_ACC_TO_REGF;
        value |= (source << 8);
        break;
    default:
        break;
    }
    return value;
}

interpreter::Instr Interpreter::parse_1(uint8_t opcode) {
    interpreter::Instr value = 0;
    switch (opcode) {
    case OpCode::RETURN:
        return value |= OpCode::RETURN;
    case OpCode::INVALID:
        return value |= OpCode::INVALID;
    case OpCode::OUTPUT:
        return value |= OpCode::OUTPUT;
    case OpCode::OUTPUTF:
        return value |= OpCode::OUTPUTF;
    default:
        return value |= OpCode::INVALID;
    }
}