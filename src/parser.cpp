#include <cctype>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../isa/instructions.h"
#include "../include/parser.h"
#include "../include/vm.h"

std::vector<interpreter::Instr> Parser::GetProgram() { return program_; }

interpreter::Instr Parser::parseOpWith2Args(uint8_t opcode, uint32_t source_1,
                                            uint32_t source_2)
{

    interpreter::Instr value = 0;

    switch (opcode)
    {
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
    case OpCode::FADD:
        value |= OpCode::FADD;
        break;
    case OpCode::FDIV:
        value |= OpCode::FDIV;
        break;
    case OpCode::FSUB:
        value |= OpCode::FSUB;
        break;
    case OpCode::FMUL:
        value |= OpCode::FMUL;
        break;
    case OpCode::FMOV_REG_TO_REG:
        value |= OpCode::FMOV_REG_TO_REG;
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

interpreter::Instr Parser::parseOpWith1Args(uint8_t opcode, uint32_t source)
{
    interpreter::Instr value = 0;
    switch (opcode)
    {
    case OpCode::INPUT:
        value |= OpCode::INPUT;
        value |= (source << 8);
        break;
    case OpCode::FINPUT:
        value |= OpCode::FINPUT;
        value |= (source << 8);
        break;
    case OpCode::FNEG:
        value |= OpCode::FNEG;
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
    case OpCode::FMOV_IMM_TO_ACC:
        value |= OpCode::FMOV_IMM_TO_ACC;
        value |= ((source >> 8) & ((1UL << 8) - 1)) << 16;
        value |= (source & ((1UL << 8) - 1)) << 24;
        break;
    case OpCode::MOV_ACC_TO_REG:
        value |= OpCode::MOV_ACC_TO_REG;
        value |= (source << 8);
        break;
    case OpCode::FMOV_ACC_TO_REG:
        value |= OpCode::FMOV_ACC_TO_REG;
        value |= (source << 8);
        break;
    default:
        break;
    }
    return value;
}

interpreter::Instr Parser::parseOpWithNoArgs(uint8_t opcode)
{
    interpreter::Instr value = 0;
    switch (opcode)
    {
    case OpCode::RETURN:
        return value |= OpCode::RETURN;
    case OpCode::INVALID:
        return value |= OpCode::INVALID;
    case OpCode::OUTPUT:
        return value |= OpCode::OUTPUT;
    case OpCode::FOUTPUT:
        return value |= OpCode::FOUTPUT;
    default:
        return value |= OpCode::INVALID;
    }
}

interpreter::OpCode Parser::tokenTypeToOpcode(TokenType id)
{
    return static_cast<OpCode>(id);
}

interpreter::RegID Parser::tokenTypeToReg(TokenType id)
{
    if (id > vm_numbers::OPCODE_NUM - 1 && id < vm_numbers::OPCODE_NUM + vm_numbers::REG_NUM - 1)
    {
        // then its int reg
        return static_cast<IRegisters>(id - vm_numbers::OPCODE_NUM);
    }
    else if (id >= vm_numbers::OPCODE_NUM + vm_numbers::REG_NUM - 1)
    {
        // std::cout << id - vm_numbers::OPCODE_NUM - vm_numbers::REG_NUM << std::endl;
        return static_cast<FRegisters>(id - vm_numbers::OPCODE_NUM - vm_numbers::REG_NUM);
    }
    return static_cast<interpreter::RegID>(TokenType::BAD_TOKEN);
}

uint64_t Parser::tokenTypeToNumber(TokenType id){
    switch (id)
    {
    case TokenType::ID_NUMBER_0:
        return 0;
    case TokenType::ID_NUMBER_1:
        return 1;
    case TokenType::ID_NUMBER_2:
        return 2;
    case TokenType::ID_NUMBER_3:
        return 3;
    case TokenType::ID_NUMBER_4:
        return 4;
    case TokenType::ID_NUMBER_5:
        return 5;
    case TokenType::ID_NUMBER_6:
        return 6;
    case TokenType::ID_NUMBER_7:
        return 7;
    case TokenType::ID_NUMBER_8:
        return 8;
    case TokenType::ID_NUMBER_9:
        return 9;
    default:
        std::cerr << "Error. Invalid digit." << std::endl;
        std::abort();
    }
}

void Parser::parseProgram(std::ifstream &file)
{
    Lexer lexer;
    std::string line;
    std::vector<uint8_t> curr_set_to_instruction = {};
    while (std::getline(file, line))
    {
        std::pair<std::vector<Token>, LexerError> tokens_and_errors = lexer.makeTokensGreatAgain(line);
        if (tokens_and_errors.second.error_ != ErrorType::STATUS_OK)
        {
            std::cerr << "Parser error." << std::endl;
            return;
        }
        int len = tokens_and_errors.first.size();
        int op_number = 0;
        for (int i = 0; i < len; ++i)
        {
            auto curr_token_type = tokens_and_errors.first[i].token_type_;
            if (curr_token_type != TokenType::BAD_TOKEN && curr_token_type < TokenType::ID_NUMBER - vm_numbers::VM_DIGIT_NUM)
            {
                if (i == 0) // On the first position always goes an operation name
                {
                    op_number++;
                    curr_set_to_instruction.push_back(tokenTypeToOpcode(curr_token_type));
                }
                else
                {
                    // Count arguments for future parsing
                    op_number++;
                    curr_set_to_instruction.push_back(tokenTypeToReg(curr_token_type));
                }
            }
            else if (curr_token_type != TokenType::BAD_TOKEN && curr_token_type >= TokenType::ID_NUMBER - vm_numbers::VM_DIGIT_NUM)
            {
                op_number++;
                curr_set_to_instruction.push_back(tokenTypeToNumber(curr_token_type));
            }
        }

        if (op_number == 3)
        {
            program_.push_back(parseOpWith2Args(curr_set_to_instruction[curr_set_to_instruction.size() - 3],
                                                curr_set_to_instruction[curr_set_to_instruction.size() - 2],
                                                curr_set_to_instruction[curr_set_to_instruction.size() - 1]));
        }
        else if (op_number == 2)
        {
            program_.push_back(
                parseOpWith1Args(curr_set_to_instruction[curr_set_to_instruction.size() - 2], curr_set_to_instruction[curr_set_to_instruction.size() - 1]));
        }
        else if (op_number == 1)
        {
            program_.push_back(parseOpWithNoArgs(curr_set_to_instruction[curr_set_to_instruction.size() - 1]));
        }
        else
        {
            std::cerr << "Error in creating instruction." << std::endl;
        }
    }

    file.close();
}