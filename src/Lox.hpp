#ifndef LOX_H
#define LOX H

#include <string>

//how are headers vs cpp files different again?

class Lox
{
  public:
  public:
    Lox() {}
    void run(std::string source);
    void run_file(std::string script);
    void run_prompt();
};

#endif
