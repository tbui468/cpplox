#include <iostream>

#include "Lox.h"
#include "Expr.h"
#include "AstPrinter.h"
#include "Interpreter.h"

//Notes on building:
  //cd build
  //cmake .. -G "Visual Studio 16 2019" -T ClangCL -A x64 #generate build using clang through visual studio
//#visual studio ignores cmake -DCMAKE_BUILD_TYPE=<Release | Debug> when generating build system, so build with following:
  //cmake --build . --config Release cl /O2 ...

//12.4 Methods on Classes
//
//  TODO:
//    Rename to Zebra
//    design how the syntax should look: static typing (checked after parsing and before interpreting)
//    Class Stmt node currently stores methods as a vector of Stmt pointers.  It really should be Function since
//      they're being cast to Function types everywhere in the Parser and Interprester anyway
//    Draw out recursive descent for expressions in Parser.  How are statments related?  Is is just logic (doesn't seem to be any recursion)
//    How is dynamic_cast, std::dynamic_pointer_cast, and std::static_pointer_cast being used in Parser and Interpreter?
//      Test this and write the results down somewhere.
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
