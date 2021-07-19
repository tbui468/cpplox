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


  void Environment::assign_at(int dis, const Token& name, std::shared_ptr<Object> value) {
    ancestor(dis)->m_values[name.m_lexeme] = value;
  }

  std::shared_ptr<Object> Environment::get(const Token& name) {
    if (m_values.count(name.m_lexeme) > 0) {
      return m_values[name.m_lexeme];
    }

    if (m_enclosing) {
      return m_enclosing->get(name);
    }

    throw RuntimeError(name, "Undefined variable '" + name.m_lexeme + "'.");
  }


  std::shared_ptr<Object> Environment::get_at(int dis, const std::string& name) {
    return ancestor(dis)->m_values[name];
  }

  std::shared_ptr<Environment> Environment::ancestor(int dis) {
    std::shared_ptr<Environment> env = shared_from_this();
    for (int i = 0; i < dis; i++) {
      env = env->m_enclosing;
    }

    return env;
  }


  void Environment::print_data() const {
        std::cout << m_values.size() << std::endl;
        std::cout << m_enclosing << std::endl;
  }
}
