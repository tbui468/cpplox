#include <iostream>

#include "Lox.hpp"

void Lox::run(std::string source)
{
  std::cout << "run" << source << std::endl;
  /*
  Scanner scanner = Scanner();
  std::vector<Token> tokens = scanner.scan_tokens();
  for(int i = 0; i < tokens.size(); i++) 
  {
    std::cout << token << '\n';
  }*/
}

void Lox::run_file(std::string script)
{
  std::cout << "running file: " << script << std::endl;
}

void Lox::run_prompt()
{
  std::cout << "running prompt" << std::endl;
}
