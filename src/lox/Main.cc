#include <iostream>

#include "Lox.h"
#include "ResultCode.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//9. Control Flow
//  9.4 For loops
//  
//  disabled for_statement for now until bug below is solved
//  Code below should throw error, but it just skips tempsss variable (which is undefined)
//
//  Found the problem.  A Runtime exeption is thrown but nothing catches it
//  so the stack goes back up one scope and continues.
//
//  Why isn't the exception thrown in the Environment propagating up to the 
/*
var temp = 0;

while (temp < 10) {
  temp = temp + 1;
  print tempsss;
}

print temp;

*/

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
