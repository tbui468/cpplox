#ifndef LOX_H
#define LOX H

#include <string>

#include "ResultCode.hpp"

namespace lox
{

class Lox
{
  private:
    inline static bool m_hadError = false;
  public:
    Lox() {}
    void run(std::string source) const;
    ResultCode run_file(std::string script) const;
    ResultCode run_prompt();
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
};

}

#endif
