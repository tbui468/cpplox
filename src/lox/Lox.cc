#include <iostream>

#include "Lox.h"
#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h"

namespace lox
{


Interpreter Lox::m_interpreter = Interpreter();

//if an error is thrown by parser,
//Lox::error is called which sets m_had_error to true
//and parser returns a nullptr
void Lox::run(std::string source) const {
  Scanner scanner = Scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();

  Parser parser = Parser(tokens); //THIS IS THE PROBLEM
  std::vector<std::unique_ptr<Stmt>> statements = parser.parse();

  if (m_had_error) return;

  Lox::m_interpreter.interpret(statements);

  //std::cout << AstPrinter().print(*expr) << std::endl;
}

ResultCode Lox::run_file(std::string script) const {
  run(script);

  if(m_had_error || m_had_runtime_error)
  {
    return ResultCode::failed;
  }else{
    return ResultCode::success;
  }
}

ResultCode Lox::run_prompt() {
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
      m_had_error = false;
    }
  }

  return ResultCode::success;
}

void Lox::error(int line, std::string message) {
  report(line, "", message);
}

void Lox::error(Token token, std::string message) {
  if (token.m_type == TokenType::EOFILE) {
    report(token.m_line, " at end", message);
  } else {
    report(token.m_line, " at '" + token.m_lexeme + "'", message);
  }
}

void Lox::runtime_error(RuntimeError error) {
  std::cout << error.get_message() << "\n[line " << std::to_string(error.get_token().m_line) << "]" << std::endl;
  m_had_runtime_error = true;  
}

void Lox::report(int line, std::string where, std::string message) {
  std::cout << "[line " << std::to_string(line) << "] Error" << where << ": " << message << std::endl;
  m_had_error = true;
}

}
