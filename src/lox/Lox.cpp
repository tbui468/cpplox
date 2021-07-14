#include <iostream>

#include "Lox.hpp"
#include "Scanner.hpp"
#include "Parser.hpp"
#include "AstPrinter.hpp"

namespace lox
{

//if an error is thrown by parser,
//Lox::error is called which sets m_had_error to true
//and parser returns a nullptr
void Lox::run(std::string source) const {
  std::cout << source << std::endl;
  Scanner scanner = Scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();

  Parser parser = Parser(tokens);
  std::unique_ptr<Expr> expr = parser.parse();

  if (m_had_error) return;

  std::cout << AstPrinter().print(*expr) << std::endl;
}

ResultCode Lox::run_file(std::string script) const {
  run(script);

  if(m_had_error)
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

void Lox::report(int line, std::string where, std::string message) {
  std::cout << "[line " << std::to_string(line) << "] Error" << where << ": " << message << std::endl;
  m_had_error = true;
}

}
