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
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the program file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        OpCode instruction = OpCode::INVALID;

        // stupid parsing. will be changed to map
        if (line == "ADD") {
            instruction = OpCode::ADD;
        } else if (line == "SUB") {
            instruction = OpCode::SUB;
        } else if(line == "MUL") {
            instruction = OpCode::MUL;
        } else if(line == "DIV") {
            instruction = OpCode::DIV;
        } else if(line == "AND") {
            instruction = OpCode::AND;
        } else if(line == "OR") {
            instruction = OpCode::OR;
        } else if(line == "XOR") {
            instruction = OpCode::XOR;
        } else if(line == "SHL") {
            instruction = OpCode::SHL;
        } else if(line == "SHR") {
            instruction = OpCode::SHR;
        } else if(line == "ASHR") {
            instruction = OpCode::ASHR;
        } else if(line == "ASHL") {
            instruction = OpCode::ASHL;
        } else if(line == "NEG") {
            instruction = OpCode::NEG;
        } else if(line == "MOV_IMM_TO_REG") {
            instruction = OpCode::MOV_IMM_TO_REG;
        } else if(line == "MOV_REG_TO_ACC") {
            instruction = OpCode::MOV_REG_TO_ACC;
        } else if(line == "INPUT") {
            instruction = OpCode::INPUT;
        } else if(line == "OUTPUT") {
            instruction = OpCode::OUTPUT;
        } else if(line == "HALT") {
            instruction = OpCode::HALT;
        } else if(line == "SIN") {
            instruction = OpCode::SIN;
        } else if(line == "COS") {
            instruction = OpCode::COS;
        } else if(line == "SQRT") {
            instruction = OpCode::SQRT;
        } else {
            std::cerr << "Error: Invalid instruction format in input file." << std::endl;
            break;
        }
        program.push_back(static_cast<int>(instruction));
    }

    file.close(); // Закрываем файл после чтения
}

void Interpreter::executeProgram() {
    int pc = 0;
    while (pc < program.size()) {
        executeInstruction(pc);
    }
}

void Interpreter::executeInstruction(int &pc) {
    OpCode opcode = static_cast<OpCode>(program[pc++]);
    int operand = program[pc++];

    switch (opcode) {
        case OpCode::ADD:
            accumulator += registers[operand];
            std::cout << accumulator << std::endl;
            break;
        case OpCode::SUB:
            accumulator -= registers[operand];
            break;
        case OpCode::MUL:
            accumulator *= registers[operand];
            break;
        case OpCode::DIV:
            accumulator /= registers[operand];
            break;
        case OpCode::NEG:
            accumulator = -accumulator;
            break;
        case OpCode::SHR:
            accumulator >>= registers[operand];
            break;
        case OpCode::SHL:
            accumulator <<= registers[operand];
            break;
        case OpCode::ASHR: {
            int shift = registers[operand];
            int sign = (accumulator < 0) ? -1 : 1;
            accumulator = sign * (abs(accumulator) >> shift);
            break;
        }
        case OpCode::ASHL: {
            int shift = registers[operand];
            accumulator = accumulator << shift;
            break;
        }
        case OpCode::AND:
            accumulator &= registers[operand];
            break;
        case OpCode::OR:
            accumulator |= registers[operand];
            break;
        case OpCode::XOR:
            accumulator ^= registers[operand];
            break;
        case OpCode::SIN:
            accumulator = std::sin(accumulator);
            break;
        case OpCode::COS:
            accumulator = std::cos(accumulator);
            break;
        case OpCode::MOV_REG_TO_ACC:
            accumulator = registers[operand];
            break;
        case OpCode::MOV_IMM_TO_REG:
            registers[operand] = operand;
            break;
        case OpCode::INPUT:
            int input;
            std::cin >> input;
            registers[operand] = input;
            break;
        case OpCode::OUTPUT:
            std::cout << registers[operand] << std::endl;
            break;
        default:
            std::cerr << "Error: Unknown opcode " << static_cast<int>(opcode) << std::endl;
            exit(1);
    }
}
