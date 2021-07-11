#include <iostream>

#include "Lox.hpp"
#include "ResultCode.hpp"
#include "Expr.hpp"
#include "AstPrinter.hpp"

//6. Parsing Expressions
//  6.1 Ambiguity and the Parsing Game
//  6.2 Recursive Descent Parsing 
//    finish implementing expression grammar for Parser

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
  lox::AstPrinter astprinter;
  lox::Literal l = lox::Literal("123");
  lox::Literal r = lox::Literal("45.67");
  
  lox::Token star_token = lox::Token(lox::TokenType::STAR, "*", "", 1);
  lox::Token minus_token = lox::Token(lox::TokenType::MINUS, "-", "", 1);

  lox::Unary u = lox::Unary(&minus_token, &l);
  lox::Grouping g = lox::Grouping(&r);
  
  lox::Binary b = lox::Binary(&u, &star_token, &g);
  std::cout << astprinter.print(b) << std::endl;


  if(result == lox::ResultCode::success) {
    return 0;
  } else {
    return 1;
  }

}
