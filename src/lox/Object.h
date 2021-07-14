#ifndef OBJECT_H
#define OBJECT_H

#include "TokenType.hpp"

namespace lox {

  class Object {
    private:
      TokenType m_type;
    public:
      std::string m_literal;
    public:
      Object(TokenType type, std::string literal): m_type(type), m_literal(literal) {}
  };

}


#endif //OBJECT_H
