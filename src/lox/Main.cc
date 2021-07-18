#include <iostream>

#include "Lox.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//10. Functions
//  todo:
//    10.4 Function Objects (functions should work now, except they can't retunr yet)
//      major leak in scope.  The following prints out 4, but it should print 6.
//      the first call to ret(5-3) changes the value of n, and the second call becomes ret(3-2) 
//      BUT it should be ret(5-3) and ret(5-3) for both.  The environments scopes aren't set up
//      correctly.  Leak somewhere with LoxFunction

/*
  fun ret(n) {
    return n;
  }

  fun test(n) {
    print ret(n-2) + ret(n-2);
  }

  test(5);

*/

//
//  TODO:
//    Combine and fix AstPrinter() so that it works on both Expr and Stmt
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
