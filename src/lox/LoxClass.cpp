#include "LoxClass.hpp"
#include "LoxInstance.hpp"

namespace lox {

  LoxClass::LoxClass(const std::string& name): Callable(), m_name(name) {}

  std::shared_ptr<Object> LoxClass::call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) {
    return std::make_shared<LoxInstance>(shared_from_this());  
  }

  int LoxClass::arity() const {
    return 0;
  }

  std::string LoxClass::to_string() const {
    return m_name;
  }

}
