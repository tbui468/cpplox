#include <iostream>

#include "Lox.hpp"
#include "ResultCode.hpp"
#include "Expr.hpp"
#include "AstPrinter.hpp"

//6. Parsing Expressions
//  6.1 Ambiguity and the Parsing Game
//  6.2 Recursive Descent Parsing 
//  6.3 Syntax errors
//    synchronize function in parser

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
