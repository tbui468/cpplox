#include "Parser.h"
#include "Lox.h"
#include "Object.h"


namespace lox {

  std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!is_at_end()) {
      statements.push_back(declaration());
    } 
    return statements;
  }


  /*
   * Statements
   */
  std::shared_ptr<Stmt> Parser::declaration() {
    try {
      if (match(TokenType::CLASS)) return class_declaration();
      if (match(TokenType::FUN)) return function_declaration("function");
      if (match(TokenType::VAR)) return var_declaration();
      return statement();
    } catch (ParseError& error) {
      synchronize();
      return nullptr;
    }
  }

  std::shared_ptr<Stmt> Parser::var_declaration() {
    Token identifier = consume(TokenType::IDENTIFIER, "Expect a variable name.");

    std::shared_ptr<Expr> expr = nullptr;
    if (match(TokenType::EQUAL)) {
      expr = expression();
    }
      
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration");
    return std::make_shared<Var>(identifier, expr);
  }

  std::shared_ptr<Stmt> Parser::class_declaration() {
    Token identifier = consume(TokenType::IDENTIFIER, "Expect a class name.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::shared_ptr<Stmt>> methods;
    
    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
      methods.emplace_back(function_declaration("method")); 
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

    return std::make_shared<Class>(identifier, methods);
  }

  //recursive descent for statements
  std::shared_ptr<Stmt> Parser::statement() {
    if (match(TokenType::IF)) return if_statement();
    if (match(TokenType::PRINT)) return print_statement();
    if (match(TokenType::WHILE)) return while_statement();
    if (match(TokenType::FOR)) return for_statement();
    if (match(TokenType::RETURN)) return return_statement();
    if (check(TokenType::LEFT_BRACE)) return std::make_shared<Block>(block()); //doing left and right paren check inside block()
    return expression_statement();
  }

  std::shared_ptr<Stmt> Parser::if_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    std::shared_ptr<Stmt> then_stmt = std::make_shared<Block>(block());

    std::shared_ptr<Stmt> else_stmt = nullptr;
    if (match(TokenType::ELSE)) {
      else_stmt = std::make_shared<Block>(block());
    }

    return std::make_shared<If>(condition, then_stmt, else_stmt);
  }

  std::shared_ptr<Stmt> Parser::print_statement() {
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(value);
  }

  std::shared_ptr<Stmt> Parser::expression_statement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Expression>(expr);
  }

  std::shared_ptr<Stmt> Parser::function_declaration(const std::string& kind) {
    
    Token name = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");

    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
      do {
        if (parameters.size() >= 255) {
          error(peek(), "Can't have more than 255 parameters.");
        }
        parameters.push_back(consume(TokenType::IDENTIFIER, "Expect parameter name."));
      } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after " + kind + " parameters.");

    std::vector<std::shared_ptr<Stmt>> body = block();

    return std::make_shared<Function>(name, parameters, body);
  }

  std::shared_ptr<Stmt> Parser::while_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = std::make_shared<Block>(block());
    
    return std::make_shared<While>(condition, body);
  }

  std::shared_ptr<Stmt> Parser::for_statement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match(TokenType::SEMICOLON)) {
      initializer = nullptr;
    } else if (match(TokenType::VAR)) {
      initializer = var_declaration();
    } else {
      initializer = expression_statement();
    }

    std::shared_ptr<Expr> condition;
    if (match(TokenType::SEMICOLON)) {
      condition = nullptr;
    } else {
      condition = expression();
      consume(TokenType::SEMICOLON, "Expect ';' after for-loop condition.");
    }

    std::shared_ptr<Expr> increment;
    if (match(TokenType::RIGHT_PAREN)) {
      increment = nullptr;
    } else {
      increment = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')' after for-loop clause.");
    }

    std::shared_ptr<Stmt> body = std::make_shared<Block>(block());

    if (increment) {
      std::vector<std::shared_ptr<Stmt>> statements;
      statements.push_back(body);
      statements.push_back(std::make_shared<Expression>(increment));
      body = std::make_shared<Block>(statements);
    }

    if (!condition) {
      condition = std::make_shared<Literal>(std::make_shared<Object>(true));
    }
    body = std::make_shared<While>(condition, body);

    if (initializer) {
      std::vector<std::shared_ptr<Stmt>> statements;
      statements.push_back(initializer);
      statements.push_back(body);
      body = std::make_shared<Block>(statements);
    }

    return body;
  }

  std::shared_ptr<Stmt> Parser::return_statement() {
    Token keyword = previous(); //the 'return' keyword

    //note: expression may be empty, eg just 'return;' to exit function
    std::shared_ptr<Expr> return_value = nullptr;
    if(!check(TokenType::SEMICOLON)) {
      return_value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");

    return std::make_shared<Return>(keyword, return_value);
  }
  
  std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;

    consume(TokenType::LEFT_BRACE, "Expect '{' to start new block.");

    while (!check(TokenType::RIGHT_BRACE) && !is_at_end()) {
      statements.push_back(declaration());
    }

    consume(TokenType::RIGHT_BRACE, "Expect '}' to end block.");

    return statements;
  }
 
  //recursive descent for expressions 
  std::shared_ptr<Expr> Parser::expression() { 
    return assignment();
  }

  std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = logic_or();

    if (match(TokenType::EQUAL)) {
      Token equals = previous();
      std::shared_ptr<Expr> value = assignment(); //not equality() since it could be followed by more assignments

      if (dynamic_cast<Variable*>(expr.get())) {
        Token name = dynamic_cast<Variable*>(expr.get())->name;
        return std::make_shared<Assign>(name, value);  
      } else if (dynamic_cast<Get*>(expr.get())) {
        std::shared_ptr<Get> get_expr = std::dynamic_pointer_cast<Get>(expr);
        return std::make_shared<Set>(get_expr->object, get_expr->name, value);
      }

      error(equals, "Invalid assignment target.");
    }

    return expr;
  }

  std::shared_ptr<Expr> Parser::logic_or() {
    std::shared_ptr<Expr> left = logic_and();
    
    if (match(TokenType::OR)) {
      Token token = previous();
      std::shared_ptr<Expr> right = logic_and();
      return std::make_shared<Logical>(token, left, right);
    }

    return left;
  }
  
  std::shared_ptr<Expr> Parser::logic_and() {
    std::shared_ptr<Expr> left = equality();

    if (match(TokenType::AND)) {
      Token token = previous();
      std::shared_ptr<Expr> right = equality();
      return std::make_shared<Logical>(token, left, right);
    }

    return left;
  }

  std::shared_ptr<Expr> Parser::equality(){
    std::shared_ptr<Expr> left = comparison();
    while (match(TokenType::BANG_EQUAL) || match(TokenType::EQUAL_EQUAL)) {
      Token token = previous();     
      std::shared_ptr<Expr> right = comparison();
      left = std::make_shared<Binary>(token, left, right);
    }
    return left;
  }

  std::shared_ptr<Expr> Parser::comparison(){
    std::shared_ptr<Expr> left = term();
    while (match(TokenType::GREATER) || match(TokenType::GREATER_EQUAL) || match(TokenType::LESS) || match(TokenType::LESS_EQUAL)) {
      Token token = previous();     
      std::shared_ptr<Expr> right = term();
      left = std::make_shared<Binary>(token, left, right);
    }
    return left;
  }

  std::shared_ptr<Expr> Parser::term(){
    std::shared_ptr<Expr> left = factor();
    while (match(TokenType::MINUS) || match(TokenType::PLUS)) {
      Token token = previous();     
      std::shared_ptr<Expr> right = factor();
      left = std::make_shared<Binary>(token, left, right);
    }
    return left;
  }

  std::shared_ptr<Expr> Parser::factor(){
    std::shared_ptr<Expr> left = unary();
    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
      Token token = previous();     
      std::shared_ptr<Expr> right = unary();
      left = std::make_shared<Binary>(token, left, right);
    }
    return left;
  }

  std::shared_ptr<Expr> Parser::unary(){
    if (match(TokenType::BANG) || match(TokenType::MINUS)) {
      Token token = previous();
      std::shared_ptr<Expr> right = unary();
      return std::make_shared<Unary>(token, right);
    } else {
      return call();
    }
  }

  //call functions and methods
  std::shared_ptr<Expr> Parser::call() {
    std::shared_ptr<Expr> expr = primary();
    
    while (true) {
      if (match(TokenType::LEFT_PAREN)) { //this allows calling functions on a return, eg my_function(arg1)(arg2);
        expr = finish_call(expr);
      } else if (match(TokenType::DOT)) {
        Token name = consume(TokenType::IDENTIFIER, "Expect property name after '.'.");
        expr = std::make_shared<Get>(expr, name);
      } else {
        break;
      }
    }

    return expr;
  }
 
  //consumes parentheses and arguments of function/method calls 
  std::shared_ptr<Expr> Parser::finish_call(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
      do {
        if (arguments.size() >= 255) {
          error(peek(), "Can't have more than 255 arguments.");
        }
        arguments.push_back(expression());
      } while (match(TokenType::COMMA));
    }

    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");

    return std::make_shared<Call>(callee, paren, arguments);
  }


  std::shared_ptr<Expr> Parser::primary(){
    if (match(TokenType::FALSE)) return std::make_shared<Literal>(std::make_shared<Object>(false));
    if (match(TokenType::TRUE)) return std::make_shared<Literal>(std::make_shared<Object>(true));
    if (match(TokenType::NIL)) return std::make_shared<Literal>(std::make_shared<Object>());

    if (match(TokenType::STRING)) {
      Token token = previous();
      return std::make_shared<Literal>(std::make_shared<Object>(token.m_literal));
    }

    if (match(TokenType::NUMBER)) {
      Token token = previous();
      return std::make_shared<Literal>(std::make_shared<Object>(std::stod(token.m_literal)));
    }

    if (match(TokenType::THIS)) {
      return std::make_shared<This>(previous());
    }

    //identifiers can be function names, class names, or variable names
    if (match(TokenType::IDENTIFIER)) {
      return std::make_shared<Variable>(previous());
    }

    if(match(TokenType::LEFT_PAREN)) {
      std::shared_ptr<Expr> expr = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')'");
      return expr;
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

  Token Parser::consume(TokenType type, const std::string& message) {
    if(check(type)) return advance();

    throw error(peek(), message);
  }

  Parser::ParseError Parser::error(Token token, const std::string& message) {
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
