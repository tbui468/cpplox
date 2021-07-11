#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.hpp"

namespace lox
{

class Token {
  public:
    TokenType m_type;
    std::string m_lexeme;
    std::string m_literal; //saving Numbers as strings. Convert to double/int if TokenType is 'Number'
    int m_line;
  public:
    Token(TokenType type, std::string lexeme, std::string literal, int line): 
      m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {}

    std::string to_string() const;
};

}


#endif //TOKEN_H
