#include "Token.hpp"

namespace lox
{

std::string Token::to_string() const {
  std::string l = m_literal == "" ? "[none]" : m_literal;
  return std::to_string(m_type) + " " + m_lexeme + " " + l;
}


}
