#ifndef INCLUDE_LEXER_H
#define INCLUDE_LEXER_H

#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include "../isa/isa_for_parser.h"

enum TokenType : uint64_t
{
#define OPLIST(code, mnemonic, format) OP_##code,
    ISA(OPLIST)
#undef OPLIST

#define INT_REGLIST(code, mnemonic, format) REG_##code,
        INT_REGS(INT_REGLIST)
#undef INT_REGLIST

#define FLOAT_REGLIST(code, mnemonic, format) REG_##code,
            FLOAT_REGS(FLOAT_REGLIST)
#undef FLOAT_REGLIST
    ID_NUMBER_0,
    ID_NUMBER_1,
    ID_NUMBER_2,
    ID_NUMBER_3,
    ID_NUMBER_4,
    ID_NUMBER_5,
    ID_NUMBER_6,
    ID_NUMBER_7,
    ID_NUMBER_8,
    ID_NUMBER_9,
    ID_NUMBER,
    BAD_TOKEN,
    ID_STRING,
    COMMA,
    SQUARE_BRACKET_L,
    SQUARE_BRACKET_R,
};

struct Token
{
    Token() : line_(""), bound_left_(0), bound_right_(0), token_type_(BAD_TOKEN) {}
    Token(std::string line, size_t bound_left, size_t bound_right, TokenType token_type)
        : line_(line), bound_left_(bound_left), bound_right_(bound_right), token_type_(token_type) {}

    std::string line_;
    size_t bound_left_;
    size_t bound_right_;
    TokenType token_type_;
};

enum ErrorType
{
    STATUS_OK,

    ERROR_UNEXPECTED_END_OF_LINE,

    ERR_NO_TOKENS,           // empty programm
    ERR_BAD_STRING,          // abbdjhd!!2222200==
    ERR_BAD_NUMBER,          // 1/0
    ERR_BAD_OPERAND,         // not from reg lists
    ERR_BAD_OPERATION_NAME,  // not from isa list
    ERR_BAD_NUMBER_OPERANDS, // according to the numer of arguments
};

struct LexerError
{

    LexerError() : error_(STATUS_OK), line_(""), position_(0), message_("") {}
    LexerError(std::string msg, size_t p, ErrorType err_type,
               std::string buff)
        : error_(err_type), line_(buff), position_(p), message_(msg)
    {
    }

    ErrorType error_;
    std::string line_;
    size_t position_;
    std::string message_;
};

typedef std::pair<std::vector<Token>, LexerError> TokensWithErrors;

class Lexer
{
public:
    Lexer() {}
    ~Lexer() = default;

    bool endOfLine();
    bool lexString();
    void lexTokens();
    void skipSpace();
    void progessSingleLine();
    bool isDigit(char c);
    bool isQuote(char c);
    TokenType whichDigit(char c);
    TokenType isSpecialSymbol(char c);
    TokenType getType(size_t bound_l, size_t bound_r);
    TokenType isOperation(std::string operation);
    TokenType isIntReg(std::string reg);
    TokenType isFloatReg(std::string reg);

    std::vector<Token> tokens_;
    std::string buffer_ {};
    size_t position_;
    size_t end_line_;
    LexerError error_;
    TokensWithErrors makeTokensGreatAgain(std::string &tokenize_string);
};

#endif // INCLUDE_LEXER_H