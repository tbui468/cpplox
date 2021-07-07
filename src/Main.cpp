#include <iostream>

#include "Lox.hpp"
#include "ResultCode.hpp"


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
