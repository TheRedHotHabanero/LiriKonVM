#include <cctype>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../isa/instructions.h"
#include "../include/parser.h"


std::vector<interpreter::Instr> Parser::GetProgram() { return program_; }

interpreter::Instr Parser::parse_3(uint8_t opcode, uint32_t source_1,
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

interpreter::Instr Parser::parse_2(uint8_t opcode, uint32_t source)
{
    interpreter::Instr value = 0;
    switch (opcode)
    {
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

interpreter::Instr Parser::parse_1(uint8_t opcode)
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
    case OpCode::OUTPUTF:
        return value |= OpCode::OUTPUTF;
    default:
        return value |= OpCode::INVALID;
    }
}

void Parser::parseProgram(std::ifstream &file)
{
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string word;
        int counter = 0;
        while (iss >> word)
        {
            uint16_t num_arg = 0;
            if (lexer_->isOperation(word) != TokenType::BAD_TOKEN)
            {
                words_.push_back(lexer_->isOperation(word));
                counter += 1;
            }
            else if (lexer_->isIntReg(word) != TokenType::BAD_TOKEN)
            {
                words_.push_back(lexer_->isIntReg(word));
                counter += 1;
            }
            else if (lexer_->isFloatReg(word) != TokenType::BAD_TOKEN)
            {
                words_.push_back(lexer_->isFloatReg(word));
                counter += 1;
            }
            else
            {
                try
                {
                    num_arg = std::stoi(word);
                    words_.push_back(num_arg);
                    counter += 1;
                }
                catch (std::invalid_argument const &ex)
                {
                }
            }
        };
        if (counter == 3)
        {
            program_.push_back(parse_3(words_[words_.size() - 3],
                                       words_[words_.size() - 2],
                                       words_[words_.size() - 1]));
        }
        else if (counter == 2)
        {
            program_.push_back(
                parse_2(words_[words_.size() - 2], words_[words_.size() - 1]));
        }
        else if (counter == 1)
        {
            program_.push_back(parse_1(words_[words_.size() - 1]));
        }
        else
        {
            std::cerr << "Error in creating instruction." << std::endl;
        }
    }

    file.close();
}