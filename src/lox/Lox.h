#ifndef LOX_H
#define LOX H

#include <string>
#include <memory>
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
    static std::shared_ptr<Interpreter> m_interpreter;
  public:
    static void run(std::string source);
    static void run_file(std::string script);
    static void run_prompt();
    static void error(int line, std::string message);
    static void report(int line, std::string where, std::string message);
    static void error(Token token, std::string message);
    static void runtime_error(RuntimeError error);
};

}

#endif
