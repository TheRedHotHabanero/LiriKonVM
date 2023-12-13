#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

#include <utility>
#include <vector>
#include <string>

#include "vm.h"
#include "instruction.h"
#include "lexer.h"

class Parser
{
public:
    Parser() : tokens_(), token_type_(TokenType::BAD_TOKEN), position_(0), error_(), instruction_() {}
    ~Parser() = default;
    std::vector<interpreter::Instr> GetProgram();
    void parseProgram(std::ifstream &file);

    std::pair<Instruction, LexerError> Parse(const std::vector<Token> &token)
    {
        tokens_ = {};
        token_type_ = TokenType::BAD_TOKEN;
        position_ = 0;
        error_ = LexerError();
        instruction_ = Instruction();

        if (token.size() == 0)
        {
            error_ = LexerError("No tokens recieved!", 0, ErrorType::ERR_NO_TOKENS,
                                "");
            return std::pair<Instruction, LexerError>(instruction_, error_);
        }

        tokens_ = token;

        if (ParseOperation())
        {
            ParseOperands();
        }

        return std::pair<Instruction, LexerError>(instruction_, error_);
    }

    std::pair<Instruction, LexerError> Parse(std::string string_to_parse)
    {
        Lexer lexer;
        auto res = lexer.TokenizeString(string_to_parse);

        if (res.second.error_ != ErrorType::STATUS_OK)
        {
            return std::pair<Instruction, LexerError>(instruction_, error_);
        }

        return Parse(res.first);
    }

    bool ParseOperation();
    bool ParseOperands();

    interpreter::Instr parse_3(uint8_t opcode, uint32_t source_1,
                               uint32_t source_2);
    interpreter::Instr parse_2(uint8_t opcode, uint32_t source);
    interpreter::Instr parse_1(uint8_t opcode);

    LexerError ShowError() const { return error_; }

private:
    std::vector<interpreter::Instr> program_ = {};
    std::vector<uint8_t> words_ = {};
    std::vector<Token> tokens_;
    TokenType token_type_;
    size_t position_;
    LexerError error_;
    Instruction instruction_;
    Lexer *lexer_;
};

#endif // INCLUDE_LEXER_H