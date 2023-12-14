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

interpreter::Instr Parser::parse_2(uint8_t opcode, uint32_t source)
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
    case OpCode::FOUTPUT:
        return value |= OpCode::FOUTPUT;
    default:
        return value |= OpCode::INVALID;
    }
}

opcode_type Parser::TokenToOpcode(TokenType id)
{
    return static_cast<OpCode>(id);
}

regs_type Parser::TokenToReg(TokenType id)
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
    return static_cast<regs_type>(TokenType::BAD_TOKEN);
}

void Parser::parseProgram(std::ifstream &file)
{
    Lexer lexer;
    std::string line;
    std::vector<uint8_t> words_ = {};
    while (std::getline(file, line))
    {
        std::pair<std::vector<Token>, LexerError> tokens_and_errors = lexer.TokenizeString(line);
        int len = tokens_and_errors.first.size();
        int counter = 0;
        std::cout << "my line is " << line << std::endl;
        std::cout << " len is " << len << std::endl;
        for (int i = 0; i < len; ++i)
        {
            std::cout << "i = " << i << std::endl;
            std::cout << "token type = " << tokens_and_errors.first[i].token_type_ << std::endl;
            auto curr_token_type = tokens_and_errors.first[i].token_type_;
            std::cout << "new token above " << std::endl;
            if (curr_token_type != TokenType::BAD_TOKEN)
            {
                if (i == 0)
                {
                    words_.push_back(TokenToOpcode(curr_token_type)); // we need togive only inst name without operands
                }
                else
                {
                    // count arguments for future parsing
                    counter += 1;
                    std::cout << "Token to reg = " << TokenToReg(curr_token_type) << std::endl;
                    words_.push_back(TokenToReg(curr_token_type));
                }
            }
        }

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