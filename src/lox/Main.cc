#include <iostream>

#include "Lox.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//11. Resolution and Binding
//
//  TODO:
//    implement native function clock()
//      call it in code like this: var time = clock(); //returns a number
//    fix warning bugs
//    consume a { outside of block() allows a more specific error message (eg, "Expect '{' before function body")
//      should do this for better error messages
//    Change file extensions to .hpp and .cpp
//    Finish filling out AstPrinter functions (many are currently stubs)

int main(int argc, char** argv) {


  if(argc > 2) {
    std::cout << "Usage: lox [script]" << std::endl;
  } else if(argc == 2) {
    lox::Lox::run_file(argv[1]);
  } else {
    lox::Lox::run_prompt();
  }

  return 0;

}
