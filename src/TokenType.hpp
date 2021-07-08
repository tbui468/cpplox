#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

namespace lox
{

//not using 'enum class' because no easy way to convert to string
enum TokenType
{
  //single char tokens (11)
  LEFT_PAREN = 0, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  //one or two char tokens (8)
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  //literals (8)
  IDENTIFIER, STRING, NUMBER,

  //keywords (16)
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  //(1)
  OEF 
};

}


#endif //TOKEN_TYPE_H
