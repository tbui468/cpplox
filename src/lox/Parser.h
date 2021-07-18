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
      std::vector<std::shared_ptr<Stmt>> parse();
    private:
      std::shared_ptr<Stmt> declaration();
      std::shared_ptr<Stmt> var_declaration();
      std::shared_ptr<Stmt> statement();
      std::shared_ptr<Stmt> if_statement();
      std::shared_ptr<Stmt> print_statement();
      std::shared_ptr<Stmt> expression_statement();
      std::shared_ptr<Stmt> while_statement();
      std::shared_ptr<Stmt> for_statement();
      std::shared_ptr<Stmt> func(const std::string& kind);
      std::shared_ptr<Stmt> return_statement();
      std::vector<std::shared_ptr<Stmt>> block();
      ParseError error(Token token, std::string message);
      std::shared_ptr<Expr> expression();
      std::shared_ptr<Expr> assignment();
      std::shared_ptr<Expr> logic_or();
      std::shared_ptr<Expr> logic_and();
      std::shared_ptr<Expr> equality();
      std::shared_ptr<Expr> comparison();
      std::shared_ptr<Expr> term();
      std::shared_ptr<Expr> factor();
      std::shared_ptr<Expr> unary();
      std::shared_ptr<Expr> finish_call(std::shared_ptr<Expr> callee);
      std::shared_ptr<Expr> call();
      std::shared_ptr<Expr> primary();
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
