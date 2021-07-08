#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "TokenType.hpp"

namespace lox
{

class Token {
  private:
    TokenType m_type;
    std::string m_lexeme;
    std::string m_literal; //convert to double if necessary (going to be slow though)
    int m_line;
  public:
    Token(TokenType type, std::string lexeme, std::string literal, int line): 
      m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {}

    std::string to_string() const;
};

}


#endif //TOKEN_H
