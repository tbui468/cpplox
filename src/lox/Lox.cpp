#include <iostream>

#include "Lox.hpp"
#include "Scanner.hpp"

namespace lox
{

void Lox::run(std::string source) const
{
  std::cout << source << std::endl;
  Scanner scanner = Scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();
  for(int i = 0; i < tokens.size(); i++) 
  {
    std::cout << tokens.at(i).to_string() << '\n';
  }
}

ResultCode Lox::run_file(std::string script) const
{
  run(script);

  if(m_hadError)
  {
    return ResultCode::failed;
  }else{
    return ResultCode::success;
  }
}

ResultCode Lox::run_prompt()
{
  std::string line;
  while(1)
  {
    std::cout << "> ";
    getline(std::cin, line);
    if(line == "quit()")
    {
      break;
    }
    else
    {
      run(line);
      m_hadError = false;
    }
  }

  return ResultCode::success;
}

void Lox::error(int line, std::string message)
{
  report(line, "", message);
}

void Lox::report(int line, std::string where, std::string message)
{
  std::cout << "[line " << std::to_string(line) << "] Error" << where << ": " << message << std::endl;
  m_hadError = true;
}

}