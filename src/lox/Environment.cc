#include "Environment.h"
#include "RuntimeError.h"

namespace lox {
  void Environment::define(const std::string& name, std::shared_ptr<Object> value) {
    if (m_values.count(name) == 0) {
      m_values[name] = value; 
    }
  }

  void Environment::assign(Token name, std::shared_ptr<Object> value) {
    if (m_values.count(name.m_lexeme) > 0) {
      m_values[name.m_lexeme] = value;
      return;
    }

    if (m_enclosing) {
      m_enclosing->assign(name, value);
      return;
    }

    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
  }

  std::shared_ptr<Object> Environment::get(Token name) {
    if (m_values.count(name.m_lexeme) > 0) {
      return m_values[name.m_lexeme];
    }

    if (m_enclosing) {
      return m_enclosing->get(name);
    }


    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
  }
}
