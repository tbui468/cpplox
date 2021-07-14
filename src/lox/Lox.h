#ifndef LOX_H
#define LOX H

#include <string>

#include "ResultCode.h"
#include "Token.h"
#include "RuntimeError.h"
#include "Interpreter.h"

namespace lox
{

class Lox
{
  private:
    inline static bool m_had_error = false;
    inline static bool m_had_runtime_error = false;
    static Interpreter m_interpreter;
  public:
    Lox() {}
    void run(std::string source) const;
    ResultCode run_file(std::string script) const;
    ResultCode run_prompt();
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
    static void error(Token token, std::string message);
    static void runtime_error(RuntimeError error);
};

}

#endif
