#include <iostream>

#include "Lox.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//Note on building:
//cd release
//cmake -G "Visual Studio 16 2019" -T ClangCL -A x64
//#visual studio ignores -DCMAKE_BUILD_TYPE=<Release | Debug> when generating build system, so build with following:
//cmake --build . --config Release cl /O2 ...

//12. Class Instances
//
//  TODO:
//    Object::get_string() should work for functions and classes
//      so when user types in print dog; (where dog is a class or function), it should print <function dog> and parameters
//      the to_string() function in LoxFunction and Callable? don't workin c++ - they're overrides in Java
//    The book allows declaring variables with same name in global scope, but this
//      really shouldn't be possible - to many potential bugs.  Don't allow it.
//    Optional: Change remaining Expr and Stmt visitor pattern to use shared_from_this() instead of *this
//      this will make everything more consistent
//    Change name of Expr visitor interfaces from VisitorObject -> ExprVisitorObject, and VisitorString -> ExprVisitorString
//      this will keep it consistent with Stmt visitor interfaces naming, and make the different interfaces between
//      Expr and Stmt more clear
//    When using vectors, use emplace_back() to construct in place (inside the vector memory space)
//    Move Clock implementation out of Interpreter class
//      make new file called NativeFunctions.h and import them into Interpreter.h
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
