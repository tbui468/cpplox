#include "LoxClass.hpp"

namespace lox {

  LoxClass::LoxClass(const std::string& name): Object(), m_name(name) {}

  std::string LoxClass::to_string() const {
    return m_name;
  }

}
