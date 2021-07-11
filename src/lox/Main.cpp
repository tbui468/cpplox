#include <iostream>

#include "Lox.hpp"
#include "ResultCode.hpp"
#include "Expr.hpp"
#include "AstPrinter.hpp"

//5. Representing Code
  //5.1 Context-Free Grammars
  //5.2 Implementing Syntax Trees (continue generating Abstract Syntax Tree types)
  //5.3 Working with Trees
  //5.4 AstPrinter (finish the parenthesizer function)
    //write out how the visitor pattern (with accept/visit and template classes with return type) is working here

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
  //make some test Expressions (Binary, Unary, Literal and Grouping)
  //shove them into astprinter and see if it works
  //literal only takes a string (or number as a string - int or double if decimal point is there)
  lox::Literal l = lox::Literal("dog");
  std::cout << astprinter.visit(l) << std::endl;

  lox::Literal l2 = lox::Literal("");
  std::cout << astprinter.visit(l2) << std::endl;


  if(result == lox::ResultCode::success) {
    return 0;
  } else {
    return 1;
  }

}
