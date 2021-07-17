#include "Parser.h"
#include "Lox.h"
#include "Object.h"


namespace lox {

  std::vector<std::unique_ptr<Stmt>> Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!is_at_end()) {
      statements.push_back(declaration());
    } 
    return statements;
  }

  //I don't like how statements and declarations are split up (is it necessary)
  //three types: declarations, statements and expressions
  std::unique_ptr<Stmt> Parser::declaration() {
    try {
      if (match(TokenType::VAR)) return var_declaration();
      return statement();
    } catch (ParseError& error) {
      synchronize();
      return nullptr;
    }
  }

  std::unique_ptr<Stmt> Parser::var_declaration() {
    Token identifier = consume(TokenType::IDENTIFIER, "Expect a variable name.");

    std::unique_ptr<Expr> expr = nullptr;
    if (match(TokenType::EQUAL)) {
      expr = expression();
    }
      
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");
    return std::make_unique<Var>(identifier, std::move(expr));
  }

  //recursive descent for statements
  std::unique_ptr<Stmt> Parser::statement() {
    if (match(TokenType::IF)) return if_statement();
    if (match(TokenType::PRINT)) return print_statement();
    if (match(TokenType::WHILE)) return while_statement();
    if (match(TokenType::FOR)) return for_statement();
    if (check(TokenType::LEFT_BRACE)) return std::make_unique<Block>(block()); //doing left and right paren check inside block()
    return expression_statement();
  }

  std::unique_ptr<Stmt> Parser::if_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    std::unique_ptr<Stmt> then_stmt = std::make_unique<Block>(block());

    std::unique_ptr<Stmt> else_stmt = nullptr;
    if (match(TokenType::ELSE)) {
      else_stmt = std::make_unique<Block>(block());
    }

    return std::make_unique<If>(std::move(condition), std::move(then_stmt), std::move(else_stmt));
  }

  std::unique_ptr<Stmt> Parser::print_statement() {
    std::unique_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<Print>(std::move(value));
  }

  std::unique_ptr<Stmt> Parser::expression_statement() {
    std::unique_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_unique<Expression>(std::move(expr));
  }

  std::unique_ptr<Stmt> Parser::while_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::unique_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::unique_ptr<Stmt> body = std::make_unique<Block>(block());
    
    return std::make_unique<While>(std::move(condition), std::move(body));
  }

  std::unique_ptr<Stmt> Parser::for_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer;
    if (match(TokenType::SEMICOLON)) {
      initializer = nullptr;
    } else if (match(TokenType::VAR)) {
      initializer = var_declaration();
    } else {
      initializer = expression_statement();
    }

    std::unique_ptr<Expr> condition;
    if (match(TokenType::SEMICOLON)) {
      condition = nullptr;
    } else {
      condition = expression();
      consume(TokenType::SEMICOLON, "Expect ';' after for-loop condition.");
    }

    std::unique_ptr<Expr> increment;
    if (match(TokenType::RIGHT_PAREN)) {
      increment = nullptr;
    } else {
      increment = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')' after for-loop clause.");
    }

    std::unique_ptr<Stmt> body = std::make_unique<Block>(block());

    if (increment) {
      std::vector<std::unique_ptr<Stmt>> statements;
      statements.push_back(std::move(body));
      statements.push_back(std::make_unique<Expression>(std::move(increment)));
      body = std::make_unique<Block>(std::move(statements));
    }

    if (!condition) {
      condition = std::make_unique<Literal>(Object(true));
    }
    body = std::make_unique<While>(std::move(condition), std::move(body));

    if (initializer) {
      std::vector<std::unique_ptr<Stmt>> statements;
      statements.push_back(std::move(initializer));
      statements.push_back(std::move(body));
      body = std::make_unique<Block>(std::move(statements));
    }

    return body;
  }
  
  std::vector<std::unique_ptr<Stmt>> Parser::block() {
    std::vector<std::unique_ptr<Stmt>> statements;

    consume(TokenType::LEFT_BRACE, "Expect '{' to start new block.");

    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
      statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' to end block.");

    return statements;
  }
 
  //recursive descent for expressions 
  std::unique_ptr<Expr> Parser::expression() { 
    return assignment();
  }

  std::unique_ptr<Expr> Parser::assignment() {
    std::unique_ptr<Expr> expr = logic_or();

    if (match(TokenType::EQUAL)) {
      Token equals = previous();
      std::unique_ptr<Expr> value = assignment(); //not equality() since it could be followed by more assignments

      if (dynamic_cast<Variable*>(expr.get())) {
        Token name = dynamic_cast<Variable*>(expr.get())->name;
        return std::make_unique<Assign>(name, std::move(value));  
      }

      error(equals, "Invalid assignment target.");
    }

    return std::move(expr);
  }

  std::unique_ptr<Expr> Parser::logic_or() {
    std::unique_ptr<Expr> left = logic_and();
    
    if (match(TokenType::OR)) {
      Token token = previous();
      std::unique_ptr<Expr> right = logic_and();
      return std::make_unique<Logical>(token, std::move(left), std::move(right));
    }

    return std::move(left);
  }
  
  std::unique_ptr<Expr> Parser::logic_and() {
    std::unique_ptr<Expr> left = equality();

    if (match(TokenType::AND)) {
      Token token = previous();
      std::unique_ptr<Expr> right = equality();
      return std::make_unique<Logical>(token, std::move(left), std::move(right));
    }

    return std::move(left);
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
      return call();
    }
  }

  
  std::unique_ptr<Expr> Parser::finish_call(std::unique_ptr<Expr> callee) {
    std::vector<std::unique_ptr<Expr>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
      do {
        if (arguments.size() >= 255) {
          error(peek(), "Can't have more than 255 arguments.");
        }
        arguments.push_back(expression());
      } while (match(TokenType::COMMA));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

     return std::make_unique<Call>(std::move(callee), paren, std::move(arguments));
  }

  std::unique_ptr<Expr> Parser::call() {
    std::unique_ptr<Expr> expr = primary();
    
    while (true) {
      if (match(LEFT_PAREN)) {
        expr = finish_call(std::move(expr));
      } else {
        break;
      }
    }

    return expr;
  }

  std::unique_ptr<Expr> Parser::primary(){
    if (match(TokenType::FALSE)) return std::make_unique<Literal>(Object(false));
    if (match(TokenType::TRUE)) return std::make_unique<Literal>(Object(true));
    if (match(TokenType::NIL)) return std::make_unique<Literal>(Object());

    if (match(TokenType::STRING)) {
      Token token = previous();
      return std::make_unique<Literal>(Object(token.m_literal));
    }

    if (match(TokenType::NUMBER)) {
      Token token = previous();
      return std::make_unique<Literal>(Object(std::stod(token.m_literal)));
    }

    if (match(TokenType::IDENTIFIER)) {
      return std::make_unique<Variable>(previous());
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
    return peek().m_type == TokenType::EOFILE;
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
