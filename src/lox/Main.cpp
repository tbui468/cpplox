#include <iostream>

#include "Lox.hpp"
#include "ResultCode.hpp"
#include "Expr.hpp"
#include "AstPrinter.hpp"
#include "Interpreter.h"

//7. Evaluating Expressions
//  7.1 Representing Values
//  7.2 Evaluating expressions
//  7.3 Runtime errors
//  TODO: make Interpretor.cc file and move definitions into there
//  74. Hooking Up the Interpreter

int main(int argc, char** argv) {

  lox::Lox cpplox;

  lox::ResultCode result = lox::ResultCode::success;

  if(argc > 2) {
    std::cout << "Usage: lox [script]" << std::endl;
    result = lox::ResultCode::failed;
  } else if(argc == 2) {
    result = cpplox.run_file(argv[1]);
  } else {
    result = cpplox.run_prompt();
  }

  //testing AstPrinter
  /*
  lox::AstPrinter astprinter;
  
  lox::Token star_token = lox::Token(lox::TokenType::STAR, "*", "", 1);
  lox::Token minus_token = lox::Token(lox::TokenType::MINUS, "-", "", 1);

  std::unique_ptr<lox::Expr> u = std::make_unique<lox::Unary>(minus_token, std::make_unique<lox::Literal>("123"));
  std::unique_ptr<lox::Expr> g = std::make_unique<lox::Grouping>(std::make_unique<lox::Literal>("45.67"));
  
  lox::Binary b = lox::Binary(star_token, std::move(u), std::move(g));
  std::cout << astprinter.print(b) << std::endl;*/


  if(result == lox::ResultCode::success) {
    return 0;
  } else {
    return 1;
  }

}
