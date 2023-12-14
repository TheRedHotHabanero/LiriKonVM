#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

#include "../include/interpreter.h"
#include "../isa/instructions.h"

std::vector<interpreter::Instr> Interpreter::GetProgram() { return parser_->GetProgram(); }

Interpreter::Interpreter()
{
    decoder_ = Decoder::Init();
    runner_ = Runner::Init();
    allocator_ = std::make_unique<ArenaAllocator>(ArenaAllocator::DEFAULT_ARENA);
}

Interpreter::~Interpreter()
{
    delete decoder_;
    delete runner_;
}

void Interpreter::loadProgram(const std::string filename)
{

    std::cout << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error: Unable to open the program file." << std::endl;
        exit(1);
    }
    std::cout << "opened" << std::endl;

    parser_->parseProgram(file);
}

interpreter::Instr Interpreter::executeInstruction(interpreter::Byte *bytecode,
                                                   interpreter::IReg pc)
{
    return *reinterpret_cast<interpreter::Instr *>(&bytecode[pc]);
}

void Interpreter::executeProgram(interpreter::Byte *bytecode)
{
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

#define NEXT()                                                         \
    pc += 4;                                                           \
    *cur_instr =                                                       \
        decoder_->decodeInstruction(executeInstruction(bytecode, pc)); \
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
    if (iregisters[cur_instr->GetSecondReg()] == 0)
    {
        std::cerr << "Error. Devision by zero." << std::endl;
        return;
    }
    iregisters[IRegisters::ACC] =
        iregisters[cur_instr->reg_id] / iregisters[cur_instr->GetSecondReg()];
    NEXT();
HANDLE_DIVF:
    if (fregisters[cur_instr->GetSecondReg()] == 0)
    {
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
    if (fregisters[cur_instr->reg_id] < 0)
    {
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
