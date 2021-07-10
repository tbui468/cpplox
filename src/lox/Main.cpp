#include <iostream>

#include "Lox.hpp"
#include "ResultCode.hpp"
#include "Expr.hpp"

//5. Representing Code
  //5.1 Context-Free Grammars
  //5.2 Implementing Syntax Trees (continue generating Abstract Syntax Tree types)

int main(int argc, char** argv)
{

  lox::Lox cpplox;

  lox::ResultCode result = lox::ResultCode::success;

  if(argc > 2)
  {
    std::cout << "Usage: lox [script]" << std::endl;
    result = lox::ResultCode::failed;
  }
  else if(argc == 2)
  {
    result = cpplox.run_file(argv[1]);
  }
  else
  {
    result = cpplox.run_prompt();
  }


  if(result == lox::ResultCode::success)
  {
    return 0;
  }
  else
  {
    return 1;
  }

}
