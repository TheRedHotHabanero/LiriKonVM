#ifndef INCLUDE_LEXER_H
#define INCLUDE_LEXER_H

#include <utility>
#include <vector>
#include <string>

#include "../isa/isa_for_parser.h"

enum TokenType
{
    BAD_TOKEN,
    ID_STRING,
    COMMA,
    SQUARE_BRACKET_L,
    SQUARE_BRACKET_R,

#define OPLIST(code, mnemonic, format) OP_##code,
    ISA(OPLIST)
#undef OPLIST

#define INT_REGLIST(code, mnemonic, format) REG_##code,
        INT_REGS(INT_REGLIST)
#undef INT_REGLIST

#define FLOAT_REGLIST(code, mnemonic, format) REG_##code,
            FLOAT_REGS(FLOAT_REGLIST)
#undef FLOAT_REGLIST
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

    ERROR_UNEXPECTED_EOL,

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

    TokensWithErrors TokenizeString(std::string &tokenize_string);

    bool Eol();
    bool LexString();
    void LexTokens();
    void SkipSpace();
    void AnalyzeLine();

    bool isQuote(char c);
    TokenType isSpecialSymbol(char c);
    TokenType getType(size_t bound_l, size_t bound_r);
    TokenType isOperation(std::string operation);
    TokenType isIntReg(std::string reg);
    TokenType isFloatReg(std::string reg);

    std::vector<Token> tokens_;
    std::string buffer_;
    size_t position_;
    size_t end_line_;
    LexerError error_;
};

#endif // INCLUDE_LEXER_H