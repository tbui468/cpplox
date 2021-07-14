#include "Parser.hpp"
#include "Expr.hpp"
#include "Lox.hpp"

/*
  LEFT_PAREN = 0, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,*/

namespace lox {

  std::unique_ptr<Expr> Parser::parse() {
    try {
      return std::move(expression());
    } catch (ParseError error) {
      return nullptr;
    }
  }
  
  std::unique_ptr<Expr> Parser::expression(){
    return std::move(equality());
  }

  std::unique_ptr<Expr> Parser::equality(){
    std::unique_ptr<Expr> left = comparison();
    while (match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
      Token token = previous();     
      std::unique_ptr<Expr> right = comparison();
      left = std::make_unique<Binary>(token, std::move(left), std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr> Parser::comparison(){
    std::unique_ptr<Expr> left = term();
    while (match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) || match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
      Token token = previous();     
      std::unique_ptr<Expr> right = term();
      left = std::make_unique<Binary>(token, std::move(left), std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr> Parser::term(){
    std::unique_ptr<Expr> left = factor();
    while (match(TokenType::MINUS) || match(TokenType::PLUS)) {
      Token token = previous();     
      std::unique_ptr<Expr> right = factor();
      left = std::make_unique<Binary>(token, std::move(left), std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr> Parser::factor(){
    std::unique_ptr<Expr> left = unary();
    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
      Token token = previous();     
      std::unique_ptr<Expr> right = unary();
      left = std::make_unique<Binary>(token, std::move(left), std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr> Parser::unary(){
    if (match(TokenType::BANG) || match(TokenType::MINUS)) {
      Token token = previous();
      std::unique_ptr<Expr> right = unary();
      return std::make_unique<Unary>(token, std::move(right));
    } else {
      return std::move(primary());
    }
  }

  //Not implemented - temp code to test if everything compiles
  std::unique_ptr<Expr> Parser::primary(){
    if (match(TokenType::FALSE)) return std::make_unique<Literal>("false");
    if (match(TokenType::TRUE)) return std::make_unique<Literal>("true");
    if (match(TokenType::NIL)) return std::make_unique<Literal>("nullptr");

    if (match(TokenType::NUMBER) || match(TokenType::STRING)) {
      Token token = previous();
      return std::make_unique<Literal>(token.m_literal);
    }

    if(match(TokenType::LEFT_PAREN)) {
      std::unique_ptr<Expr> expr = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')'");
      return std::move(expr);
    }

    throw error(peek(), "Expected expression.");
  }


  bool Parser::match(TokenType type) {
    Token token = m_tokens.at(m_current);
    if(check(type)) {
      advance();
      return true;
    }else{
      return false;
    }
  }

  Token Parser::consume(TokenType type, std::string message) {
    if(check(type)) return advance();

    throw error(peek(), message);
  }

  Parser::ParseError Parser::error(Token token, std::string message) {
    Lox::error(token, message);
    return Parser::ParseError();
  }

  Token Parser::advance() {
    if (!is_at_end()) m_current++;
    return previous();
  }

  bool Parser::check(TokenType type) {
    if (is_at_end()) return false;
    return peek().m_type == type;
  }

  Token Parser::peek() const {
    return m_tokens.at(m_current);
  }

  bool Parser::is_at_end() const {
    return m_current >= m_tokens.size();
  }

  Token Parser::previous() const {
    return m_tokens.at(m_current - 1);
  }

  //discard tokens until the next statement is reached
  void Parser::synchronize() {
    advance();
    while (!is_at_end()) {
      if (previous().m_type == TokenType::SEMICOLON) {
        return;
      } else {
        switch(peek().m_type) {
          case TokenType::CLASS:
          case TokenType::FUN:
          case TokenType::VAR:
          case TokenType::FOR:
          case TokenType::IF:
          case TokenType::WHILE:
          case TokenType::PRINT:
          case TokenType::RETURN:
            return;
        }
        advance();
      }
    }
  }
}
