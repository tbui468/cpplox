#include "LoxInstance.hpp"

namespace lox {


  std::shared_ptr<Object> LoxInstance::get(const Token& name) {
    if(m_fields.count(name.m_lexeme) > 0) {
      return m_fields[name.m_lexeme];
    }

    std::shared_ptr<LoxFunction> method = m_class->find_method(name.m_lexeme);
    if(method) {
      return method->bind(shared_from_this());
    }

    throw RuntimeError(name, "Undefined property '" + name.m_lexeme + "'.");
  }


  void LoxInstance::set(const Token& name, std::shared_ptr<Object> value) {
    m_fields[name.m_lexeme] = value; 
  }

}
