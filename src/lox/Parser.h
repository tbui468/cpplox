#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <exception>
#include "Token.h"
#include "TokenType.h"
#include "Expr.h"
#include "Stmt.h"

namespace lox {

  class Parser {
    private:
      class ParseError: public std::exception {};
    private:
      std::vector<Token> m_tokens;
      int m_current = 0;
    public:
      Parser(const std::vector<Token>& tokens): m_tokens(tokens) {}
      std::vector<std::unique_ptr<Stmt>> parse();
    private:
      std::unique_ptr<Stmt> declaration();
      std::unique_ptr<Stmt> var_declaration();
      std::unique_ptr<Stmt> statement();
      std::unique_ptr<Stmt> if_statement();
      std::unique_ptr<Stmt> print_statement();
      std::unique_ptr<Stmt> expression_statement();
      std::vector<std::unique_ptr<Stmt>> block();
      ParseError error(Token token, std::string message);
      std::unique_ptr<Expr> expression();
      std::unique_ptr<Expr> assignment();
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
      Token consume(TokenType type, std::string messsage);
      void synchronize();
  };

}

#endif //PARSER_H
