#include "LoxClass.hpp"
#include "LoxInstance.hpp"

namespace lox {

  LoxClass::LoxClass(const std::string& name, std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods):
    Callable(), m_name(name), m_methods(methods) {}

  std::shared_ptr<Object> LoxClass::call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) {
    return std::make_shared<LoxInstance>(shared_from_this());  
  }

  int LoxClass::arity() const {
    return 0;
  }

  std::string LoxClass::to_string() const {
    return m_name;
  }

  std::shared_ptr<LoxFunction> LoxClass::find_method(const std::string& name) {
    if (m_methods.count(name) > 0) {
      return m_methods[name];
    }

    return nullptr;
  }

}
