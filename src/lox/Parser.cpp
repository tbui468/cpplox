#include "Parser.hpp"
#include "Expr.hpp"

/*
  LEFT_PAREN = 0, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,*/

namespace lox {
  
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
      match(TokenType::RIGHT_PAREN); //expect ')' at some point
      return std::move(expr);
    }

    //syntax error otherwise
  }


  bool Parser::match(TokenType type) {
    Token token = m_tokens.at(m_current);
    if(token.m_type == type) {
      m_current++;
      return true;
    }else{
      return false;
    }
  }

  Token Parser::previous() const {
    return m_tokens.at(m_current - 1);
  }
}
