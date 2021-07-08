#include "Token.hpp"

namespace lox
{

std::string Token::to_string() const {
  return std::to_string(m_type) + " " + m_lexeme + " " + m_literal;
}


}
