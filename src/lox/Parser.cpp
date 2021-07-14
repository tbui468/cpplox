#include "Parser.hpp"
#include "Expr.hpp"

namespace lox {
  
  Expr* Parser::expression(){
    return equality();
  }
  Expr* Parser::equality(){
    Expr* left = comparison();
    while (match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
      Token token = previous();     
    }

    return left;
  }
  Expr* Parser::comparision(){
    //this calls term
  }
  Expr* Parser::term(){
    //this calls factor
  }
  Expr* Parser::factor(){
    //this calls unary
  }
  Expr* Parser::unary(){
    //this calls primary
  }
  Expr* Parser::primary(){
    //these and the terminals
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
