#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.hpp"
#include "TokenType.hpp"
#include "Expr.hpp"

namespace lox {

  class Parser {
    private:
      std::vector<Token> m_tokens;
      int m_current = 0;
    public:
      Parser(const std::vector<Token>& tokens): m_tokens(tokens) {}
    private:
      
      //expression grammar rules
      Expr* expression();
      Expr* equality();
      Expr* comparision();
      Expr* term();
      Expr* factor();
      Expr* unary();
      Expr* primary();
      bool match(TokenType type);
      Token previous() const;
  };

}

#endif //PARSER_H
