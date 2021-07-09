#ifndef SCANNER_H
#define SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Token.hpp"
#include "TokenType.hpp"

namespace lox
{

class Scanner
{
  private:
    std::string m_source;
    std::vector<Token> m_tokens;
    int m_start = 0;
    int m_current = 0;
    int m_line = 1;
    std::unordered_map<std::string, TokenType> m_reserved;
  public:
    Scanner(std::string source): m_source(source) 
    {
      m_reserved["and"] =     TokenType::AND;
      m_reserved["class"] =   TokenType::CLASS;
      m_reserved["else"] =    TokenType::ELSE;
      m_reserved["false"] =   TokenType::FALSE;
      m_reserved["for"] =     TokenType::FOR;
      m_reserved["fun"] =     TokenType::FUN;
      m_reserved["if"] =      TokenType::IF;
      m_reserved["nil"] =     TokenType::NIL;
      m_reserved["or"] =      TokenType::OR;
      m_reserved["print"] =   TokenType::PRINT;
      m_reserved["return"] =  TokenType::RETURN;
      m_reserved["super"] =   TokenType::SUPER;
      m_reserved["this"] =    TokenType::THIS;
      m_reserved["true"] =    TokenType::TRUE;
      m_reserved["var"] =     TokenType::VAR;
      m_reserved["while"] =   TokenType::WHILE;
    }
    std::vector<Token> scan_tokens();
    void scan_token();
  private:
    bool is_at_end();
    char advance();
    void add_token(TokenType type);
    void add_token(TokenType type, std::string literal);
    bool match(char expected);
    char peek_one();
    char peek_two();
    bool is_digit(char c);
    bool is_alpha(char c);
    bool is_alpha_numeric(char c);
    void find_number();
    void find_string();
    void find_identifier();
};

}

#endif //SCANNER_H
