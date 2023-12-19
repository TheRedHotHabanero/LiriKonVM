#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

#include <string>
#include <utility>
#include <vector>

#include "instruction.h"
#include "lexer.h"
#include "vm.h"

class Parser {
public:
  Parser()
      : tokens_(), token_type_(TokenType::BAD_TOKEN), position_(0), error_(),
        instruction_() {}
  ~Parser() = default;
  std::vector<interpreter::Instr> GetProgram();
  uint64_t tokenTypeToNumber(TokenType id);
  interpreter::OpCode tokenTypeToOpcode(TokenType id);
  interpreter::RegID tokenTypeToReg(TokenType id);
  void parseProgram(std::ifstream &file);

  interpreter::Instr parseOpWith2Args(uint8_t opcode, uint32_t source_1,
                                      uint32_t source_2);
  interpreter::Instr parseOpWith1Args(uint8_t opcode, uint32_t source);
  interpreter::Instr parseOpWithNoArgs(uint8_t opcode);

  LexerError showError() const { return error_; }

private:
  std::vector<interpreter::Instr> program_ = {};
  std::vector<Token> tokens_;
  TokenType token_type_;
  size_t position_;
  LexerError error_;
  Instruction instruction_;
  Lexer lexer_;
};

#endif // INCLUDE_LEXER_H
