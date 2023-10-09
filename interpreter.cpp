#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "interpreter.h"
#include "instructions.h"

Interpreter::Interpreter() {
    registers.resize(regNum, 0); // 8 registers
    accumulator = 0;
}

Interpreter::~Interpreter() {

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
            uint64_t instruction = static_cast<uint64_t>(OpCode::INVALID);

            // Преобразование слова в инструкцию
            if (word == "ADD") {
                instruction = static_cast<uint64_t>(OpCode::ADD);
            } else if (word == "SUB") {
                instruction = static_cast<uint64_t>(OpCode::SUB);
            } else if (word == "MUL") {
                instruction = static_cast<uint64_t>(OpCode::MUL);
            } else if (word == "DIV") {
                instruction = static_cast<uint64_t>(OpCode::DIV);
            } else if (word == "AND") {
                instruction = static_cast<uint64_t>(OpCode::AND);
            } else if (word == "OR") {
                instruction = static_cast<uint64_t>(OpCode::OR);
            } else if (word == "XOR") {
                instruction = static_cast<uint64_t>(OpCode::XOR);
            } else if (word == "SHL") {
                instruction = static_cast<uint64_t>(OpCode::SHL);
            } else if (word == "SHR") {
                instruction = static_cast<uint64_t>(OpCode::SHR);
            } else if (word == "ASHR") {
                instruction = static_cast<uint64_t>(OpCode::ASHR);
            } else if (word == "ASHL") {
                instruction = static_cast<uint64_t>(OpCode::ASHL);
            } else if (word == "NEG") {
                instruction = static_cast<uint64_t>(OpCode::NEG);
            } else if (word == "MOV_IMM_TO_REG") {
                instruction = static_cast<uint64_t>(OpCode::MOV_IMM_TO_REG);
            } else if (word == "MOV_REG_TO_ACC") {
                instruction = static_cast<uint64_t>(OpCode::MOV_REG_TO_ACC);
            } else if (word == "INPUT") {
                instruction = static_cast<uint64_t>(OpCode::INPUT);
            } else if (word == "OUTPUT") {
                instruction = static_cast<uint64_t>(OpCode::OUTPUT);
            } else if (word == "HALT") {
                instruction = static_cast<uint64_t>(OpCode::HALT);
            } else if (word == "SIN") {
                instruction = static_cast<uint64_t>(OpCode::SIN);
            } else if (word == "COS") {
                instruction = static_cast<uint64_t>(OpCode::COS);
            } else if (word == "SQRT") {
                instruction = static_cast<uint64_t>(OpCode::SQRT);
            } else if (word == "r0") {
                instruction = static_cast<uint64_t>(Cells::R0);
            } else if (word == "r1") {
                instruction = static_cast<uint64_t>(Cells::R1);
            } else if (word == "r2") {
                instruction = static_cast<uint64_t>(Cells::R2);
            } else if (word == "r3") {
                instruction = static_cast<uint64_t>(Cells::R3);
            } else if (word == "r4") {
                instruction = static_cast<uint64_t>(Cells::R4);
            } else if (word == "r5") {
                instruction = static_cast<uint64_t>(Cells::R5);
            } else if (word == "r6") {
                instruction = static_cast<uint64_t>(Cells::R6);
            } else if (word == "r7") {
                instruction = static_cast<uint64_t>(Cells::R7);
            } else if (word == "IMM") {
                instruction = static_cast<uint64_t>(Cells::IMM);
            } else if (word == "ACC") {
                instruction = static_cast<uint64_t>(Cells::ACC);
            } else {
                std::cerr << "Error: Invalid instruction format in input file." << std::endl;
                std::cout << word << std::endl;
                exit(1);
            }
            program.push_back(instruction);
        }
    }

    file.close(); // Закрываем файл после чтения
}

void Interpreter::executeProgram() {
    uint64_t curr_inst = 0;
    uint64_t pc = 0;
    uint64_t size = program.size();
    while (curr_inst < size) {
        executeInstruction(pc);
        curr_inst += 1;
    }
}

void Interpreter::executeInstruction(uint64_t &pc) {

    static void *dispatch_table[] = {
        &&HANDLE_ADD,
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
        &&HANDLE_MOV_IMM_TO_REG,
        &&HANDLE_MOV_REG_TO_ACC,
        &&HANDLE_INPUT,
        &&HANDLE_OUTPUT,
        &&HALT,
        &&HANDLE_SIN,
        &&HANDLE_COS,
        &&HANDLE_SQRT,
        &&HANDLE_INVALID
    };
/*
    if (!handler.IsPrimaryOpcodeValid()) {
        LOG(ERROR, VERIFIER) << "Incorrect opcode";
        return VerificationStatus::ERROR;
    }
    goto* dispatch_table[handler.GetPrimaryOpcode()];
*/

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
HANDLE_MOV_IMM_TO_REG:
    registers[operand] = operand;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_MOV_REG_TO_ACC:
    std::cout << "in mov reg to acc. " << std::endl;
    accumulator = registers[operand];
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_INPUT:
    std::cout << "in input. " << std::endl;
    uint64_t input;
    std::cin >> input;
    accumulator = input;
    operand = program[++pc];
    goto *dispatch_table[operand];
HANDLE_OUTPUT:
    std::cout << "in output. " << std::endl;
    std::cout << accumulator << std::endl;
    operand = program[++pc];
    goto *dispatch_table[operand];
HALT:
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
