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
            int instruction = static_cast<int>(OpCode::INVALID);

            // Преобразование слова в инструкцию
            if (word == "ADD") {
                instruction = static_cast<int>(OpCode::ADD);
            } else if (word == "SUB") {
                instruction = static_cast<int>(OpCode::SUB);
            } else if (word == "MUL") {
                instruction = static_cast<int>(OpCode::MUL);
            } else if (word == "DIV") {
                instruction = static_cast<int>(OpCode::DIV);
            } else if (word == "AND") {
                instruction = static_cast<int>(OpCode::AND);
            } else if (word == "OR") {
                instruction = static_cast<int>(OpCode::OR);
            } else if (word == "XOR") {
                instruction = static_cast<int>(OpCode::XOR);
            } else if (word == "SHL") {
                instruction = static_cast<int>(OpCode::SHL);
            } else if (word == "SHR") {
                instruction = static_cast<int>(OpCode::SHR);
            } else if (word == "ASHR") {
                instruction = static_cast<int>(OpCode::ASHR);
            } else if (word == "ASHL") {
                instruction = static_cast<int>(OpCode::ASHL);
            } else if (word == "NEG") {
                instruction = static_cast<int>(OpCode::NEG);
            } else if (word == "MOV_IMM_TO_REG") {
                instruction = static_cast<int>(OpCode::MOV_IMM_TO_REG);
            } else if (word == "MOV_REG_TO_ACC") {
                instruction = static_cast<int>(OpCode::MOV_REG_TO_ACC);
            } else if (word == "INPUT") {
                instruction = static_cast<int>(OpCode::INPUT);
            } else if (word == "OUTPUT") {
                instruction = static_cast<int>(OpCode::OUTPUT);
            } else if (word == "HALT") {
                instruction = static_cast<int>(OpCode::HALT);
            } else if (word == "SIN") {
                instruction = static_cast<int>(OpCode::SIN);
            } else if (word == "COS") {
                instruction = static_cast<int>(OpCode::COS);
            } else if (word == "SQRT") {
                instruction = static_cast<int>(OpCode::SQRT);
            } else if (word == "r0") {
                instruction = static_cast<int>(Cells::R0);
            } else if (word == "r1") {
                instruction = static_cast<int>(Cells::R1);
            } else if (word == "r2") {
                instruction = static_cast<int>(Cells::R2);
            } else if (word == "r3") {
                instruction = static_cast<int>(Cells::R3);
            } else if (word == "r4") {
                instruction = static_cast<int>(Cells::R4);
            } else if (word == "r5") {
                instruction = static_cast<int>(Cells::R5);
            } else if (word == "r6") {
                instruction = static_cast<int>(Cells::R6);
            } else if (word == "r7") {
                instruction = static_cast<int>(Cells::R7);
            } else if (word == "IMM") {
                instruction = static_cast<int>(Cells::IMM);
            } else if (word == "ACC") {
                instruction = static_cast<int>(Cells::ACC);
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
    int pc = 0;
    while (pc < program.size()) {
        executeInstruction(pc);
        pc++;
    }
}

void Interpreter::executeInstruction(int &pc) {
    OpCode opcode = static_cast<OpCode>(program[pc]);
    int operand = program[pc];

    switch (opcode) {
        case OpCode::ADD:
            accumulator += registers[operand];
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
            std::cout << "in mov reg to acc. " << std::endl;
            accumulator = registers[operand];
            break;
        case OpCode::MOV_IMM_TO_REG:
            registers[operand] = operand;
            break;
        case OpCode::INPUT:
            std::cout << "in input. " << std::endl;
            int input;
            std::cin >> input;
            accumulator = input;
            break;
        case OpCode::OUTPUT:
            std::cout << "in output. " << std::endl;
            std::cout << accumulator << std::endl;
            break;
        case OpCode::HALT:
            return;
        default:
            std::cerr << "Error: Unknown opcode " << static_cast<int>(opcode) << std::endl;
            exit(1);
    }
}
