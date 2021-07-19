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

//11. Resolution and Binding
//  problem with m_locals in Interpreter.
//    we need to save reference to expression nodes, but that can't be done with maps,
//    BUT it can be done with pointers (smart or otherwise)
//    REDO VISITOR PATTERN
  //    Replace the arguments in visitor.visit(*this) with visitor.visit(shared_from_this())
  //      do for Assign and Variable since those are the only ones we need for now
  //      try resolver
  //    have Expr and Stmt inherit from std::enable_shared_from_this<Expr>, std::enable_shared_from_this<Stmt>
  //    Run tests and make sure all tests (exept for last) pass
  //    Commit with message: "Redid visitor pattern with shared_ptrs so that AST data can be saved to Interpreter"
//
//  TODO:
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
