#include <iostream>
#include <fstream>

#include "Lox.h"
#include "Scanner.h"
#include "Parser.h"
#include "AstPrinter.h"
#include "Resolver.hpp"

namespace lox {


std::shared_ptr<Interpreter> Lox::m_interpreter = std::make_shared<Interpreter>();

void Lox::run(std::string source) {
  Scanner scanner = Scanner(source);
  std::vector<Token> tokens = scanner.scan_tokens();

  Parser parser = Parser(tokens);
  std::vector<std::shared_ptr<Stmt>> statements = parser.parse();

  if (m_had_error) return;

  Resolver resolver(Lox::m_interpreter);
  resolver.resolve(statements); 


/*  
  for (const std::shared_ptr<Stmt>& stmt: statements) {
    std::cout << AstPrinter().print(*stmt) << std::endl;
  }*/

  if (m_had_error) return;


  Lox::m_interpreter->interpret(statements);

}

void Lox::run_file(std::string file_path) {
  std::string line;
  std::ifstream my_file(file_path);

  std::string all = "";

  if(my_file.is_open()) {
    while (getline(my_file, line)) {
      all += line + "\n";
    }
    my_file.close();
  }

  run(all);

  if(m_had_error || m_had_runtime_error) {
    std::cout << "Errors found in file." << std::endl;
  }
}

void Lox::run_prompt() {
  std::string line;
  while(1) {
    std::cout << "> ";
    getline(std::cin, line);
    if(line == "quit()") {
      break;
    } else {
      run(line);
      m_had_error = false;
    }
  }

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
