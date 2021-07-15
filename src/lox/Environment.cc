#include "Environment.h"
#include "RuntimeError.h"


namespace lox {
  void Environment::define(std::string name, Object value) {
    m_values[name] = value;
  }

  void Environment::assign(Token name, Object value) {
    if (m_values.count(name.m_lexeme) > 0) {
      m_values[name.m_lexeme] = value;
      return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
  }

  Object Environment::get(Token name) {
    if (m_values.count(name.m_lexeme) > 0) {
      return m_values[name.m_lexeme];
    }

    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
  }
}
