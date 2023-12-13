#include <cctype>
#include <string>
#include <unordered_map>

#include "../include/lexer.h"

TokenType Lexer::isSpecialSymbol(char c)
{
    static const std::unordered_map<char, TokenType> specials = {
        {',', TokenType::COMMA},
        {'[', TokenType::SQUARE_BRACKET_L},
        {']', TokenType::SQUARE_BRACKET_R},
    };
    return (specials.find(c) == specials.end()) ? (TokenType::BAD_TOKEN)
                                                : (specials.at(c));
}

TokenType Lexer::isOperation(std::string operation)
{
    static const std::unordered_map<std::string, TokenType> operation_list = {
#define OPLIST(code, mnemonic, format) {mnemonic, TokenType::OP_##code},
        ISA(OPLIST)
#undef OPLIST
    };

    return (operation_list.find(operation) == operation_list.end()) ? (TokenType::BAD_TOKEN)
                                                                    : (operation_list.at(operation));
}

TokenType Lexer::isIntReg(std::string reg)
{
    static const std::unordered_map<std::string, TokenType> int_reg_list = {
#define INT_REGLIST(code, mnemonic, format) {mnemonic, TokenType::REG_##code},
        INT_REGS(INT_REGLIST)
#undef INT_REGLIST
    };

    return (int_reg_list.find(reg) == int_reg_list.end()) ? (TokenType::BAD_TOKEN)
                                                          : (int_reg_list.at(reg));
}

TokenType Lexer::isFloatReg(std::string reg)
{
    static const std::unordered_map<std::string, TokenType> float_reg_list = {
#define FLOAT_REGLIST(code, mnemonic, format) {mnemonic, TokenType::REG_##code},
        FLOAT_REGS(FLOAT_REGLIST)
#undef FLOAT_REGLIST
    };

    return (float_reg_list.find(reg) == float_reg_list.end()) ? (TokenType::BAD_TOKEN)
                                                              : (float_reg_list.at(reg));
}

void Lexer::SkipSpace()
{
    while (!Eol() && isspace(buffer_[position_]) != 0)
    {
        ++position_;
    }
}

bool Lexer::isQuote(char c) { return c == '"'; }
bool Lexer::Eol() { return position_ == end_line_; }

TokenType Lexer::getType(size_t beg, size_t end)
{
    TokenType type = isOperation(buffer_.substr(beg, end - beg));
    if (type != TokenType::BAD_TOKEN)
    {
        return type;
    }

    type = Lexer::isIntReg(buffer_.substr(beg, end - beg));
    if (type != TokenType::BAD_TOKEN)
    {
        return type;
    }

    type = Lexer::isFloatReg(buffer_.substr(beg, end - beg));
    if (type != TokenType::BAD_TOKEN)
    {
        return type;
    }

    if (Lexer::isQuote(buffer_[beg]))
    {
        return TokenType::ID_STRING;
    }

    return TokenType::BAD_TOKEN;
}

TokensWithErrors Lexer::TokenizeString(std::string &s)
{
    buffer_ = s;
    position_ = 0;
    end_line_ = s.size();
    error_ = LexerError();
    tokens_ = {};

    AnalyzeLine();
    return std::pair<std::vector<Token>, LexerError>(tokens_, error_);
}

bool Lexer::LexString()
{
    bool is_esc_seq = false;
    while (!Eol())
    {
        ++position_;

        if (is_esc_seq)
        {
            is_esc_seq = false;
            continue;
        }

        char c = buffer_[position_];

        if (c == '\\')
        {
            is_esc_seq = true;
        }

        if (Lexer::isQuote(c))
        {
            break;
        }
    }

    if (!isQuote(buffer_[position_]))
    {
        error_ = LexerError("no termination for string.", position_,
                            ErrorType::ERROR_UNEXPECTED_EOL,
                            buffer_);

        return false;
    }

    ++position_;

    return true;
}

void Lexer::LexTokens()
{
    if (Eol())
    {
        return;
    }

    while (end_line_ > position_ && isspace(buffer_[end_line_ - 1] != 0))
    {
        --end_line_;
    }

    while (isspace(buffer_[position_] != 0 && !Eol()))
    {
        ++position_;
    }

    size_t bound_l = 0;
    size_t bound_r = 0;

    for (int i = 0; !Eol(); ++i)
    {
        bound_l = position_;

        if (isSpecialSymbol(buffer_[position_]) != TokenType::BAD_TOKEN)
        {
            ++position_;
        }
        else if (Lexer::isQuote(buffer_[position_]))
        {
            if (!LexString())
            {
                return;
            }
        }
        else
        {
            while (!Eol() && Lexer::isSpecialSymbol(buffer_[position_]) == TokenType::BAD_TOKEN &&
                   isspace(buffer_[position_]) == 0)
            {
                ++position_;
            }
        }

        bound_r = position_;

        tokens_.emplace_back(buffer_, bound_l, bound_r, Lexer::getType(bound_l, bound_r));

        while (isspace(buffer_[position_]) != 0 && !Eol())
        {
            ++position_;
        }
    }

    return;
}

void Lexer::AnalyzeLine()
{
    SkipSpace();
    LexTokens();

    return;
}
