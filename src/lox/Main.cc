#include <iostream>

#include "Lox.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//10. Functions
//  todo:
//    10.4 Function Objects (functions should work now, except they can't retunr yet)
//      Problem with LoxFunction object requiring a reference to Function
//      but we need a pointer since the unique_ptrs needs change ownership to LoxFunction
//      getting error involving deleted constructor for unique_ptrs
//      Interpreter is visiting Function stmt, which then instantiates LoxFunction(stmt)
//      LoxFunction needs to keep a copy of stmt to call later, BUT the unique_ptrs
//      in stmt can't be copied over since
//
//      option 1: change ALL unique_ptrs to shared_ptrs: this will take a while but should solve all problems
//      option 2: could Function body be change to shared_ptr only?  So keep other unique_ptrs
//            when Function node is created in Parser, the vector of unique_ptr<Stmt> is created by calling block()
//            Instead of calling block(), could run the same code but have it produce a vector of shared_ptr<Stmt> instead
//            can this be used to init Function?  Or will the unique_ptrs further down the tree still prevent this?
//            could try this on Godbolt's compiler explorer to check if unique_ptrs closer to the leaves will 
//            prevent use of shared_ptrs closer to the root
//
//  TODO:
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
