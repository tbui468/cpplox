#include <iostream>

void runFile(char* script)
{
  std::cout << "running file: " << script << std::endl;
}

void runPrompt()
{
  std::cout << "running prompt" << std::endl;
}

int main(int argc, char** argv)
{
  if(argc > 2)
  {
    std::cout << "Usage: lox [script]" << std::endl;
    return 1;
  }
  else if(argc == 2)
  {
    runFile(argv[1]);
  }
  else
  {
    runPrompt();
  }
  return 0;
}
