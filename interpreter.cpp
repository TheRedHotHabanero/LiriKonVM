#include "interpreter.h"
#include "instructions.h"

Interpreter::Interpreter() {
    registers.resize(8, 0); // 8 registers
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

    int instruction;
    while (file.read(reinterpret_cast<char*>(&instruction), sizeof(int))) {
        program.push_back(instruction);
        std::cout << program.size() << std::endl;
    }

    file.close();
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
        case OpCode::ADD: // ADD
            accumulator += registers[operand];
            break;
        case OpCode::SUB: // SUB
            accumulator -= registers[operand];
            break;
        case OpCode::NEG: // NEG
            accumulator = -accumulator;
            break;
        case OpCode::MOV_REG_TO_ACC: // MOV_REG_TO_ACC
            accumulator = registers[operand];
            break;
        case OpCode::MOV_IMM_TO_REG: // MOV_IMM_TO_REG
            registers[operand] = program[pc++];
            break;
        case OpCode::INTRINSIC_IO: // INTRINSIC_IO
            if (operand == 0) {
                std::cout << accumulator << std::endl;
            } else if (operand == 1) {
                std::cin >> registers[0];
            }
            break;
    }
}
