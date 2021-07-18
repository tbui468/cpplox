#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.h"

namespace lox
{

class Token {
  public:
    TokenType m_type;
    std::string m_lexeme;
    std::string m_literal;
    int m_line;
  public:
    Token(TokenType type, const std::string& lexeme, const std::string& literal, int line): 
      m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {}

    std::string to_string() const;
};

}


#endif //TOKEN_H
