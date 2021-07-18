#include <iostream>

#include "Lox.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//10. Functions
//  todo:
//    10.4 Function Objects (functions should work now, except they can't retunr yet)
//      
//      problem with function not being callable
//        'getting only functions and methods can be called' exception in Interpreter
//        the dynamic cast is not working when an Object is returned
//
//      solution:
//        change all Object return types to std::shared_ptr<Object>
//        Expr.h
//        Interpreter.h / Interpreter.cpp
//
//  TODO:
//    Change file extensions to .hpp and .cpp
//    fix warning bugs
//    Clean Up - rule of five, const and const/ref, etc
//      Lox: done
//      Scanner: done
//      Parser: 
//      Interpreter:
//    native function clock() is not implemented yet (since we can't use return values of functions yet)
//    consume a { outside of block() allows a more specific error message (eg, "Expect '{' before function body")
//      should do this for better error messages
//    m_globals in Interpreter should be const since we don't want anyone to change where it points to

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
