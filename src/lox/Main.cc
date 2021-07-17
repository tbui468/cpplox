#include <iostream>

#include "Lox.h"
#include "ResultCode.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//10. Functions
//  todo:
//    10.4 Function Objects (functions should work now, except they can't retunr yet)
//    native function clock() is not implemented yet (since we can't use return values of functions yet)
//    Function stmt and Function object have clashing names!!!
//    consume a { outside of block() allows a more specific error message (eg, "Expect '{' before function body")
//      should do this for better error messages
//    m_globals in Interpreter should be const since we don't want anyone to change where it points to

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
