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
      std::unique_ptr<Expr> expression();
      std::unique_ptr<Expr> equality();
      std::unique_ptr<Expr> comparison();
      std::unique_ptr<Expr> term();
      std::unique_ptr<Expr> factor();
      std::unique_ptr<Expr> unary();
      std::unique_ptr<Expr> primary();
      bool match(TokenType type);
      Token previous() const;
      bool check(TokenType type);
      Token peek() const;
      bool is_at_end() const;
      Token advance();
  };

}

#endif //PARSER_H
