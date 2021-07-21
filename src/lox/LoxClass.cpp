#include "LoxClass.hpp"
#include "LoxInstance.hpp"

namespace lox {

  LoxClass::LoxClass(const std::string& name, 
                     std::shared_ptr<LoxClass> superclass,
                     std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods):
                      Callable(), m_name(name), m_superclass(superclass), m_methods(methods) {}

  std::shared_ptr<Object> LoxClass::call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) {
    std::shared_ptr<LoxInstance> instance = std::make_shared<LoxInstance>(shared_from_this());  
    std::shared_ptr<LoxFunction> initializer = find_method("init");
    if(initializer) {
      initializer->bind(instance)->call(interp, arguments);
    }
    return instance;
  }

  int LoxClass::arity() {
    std::shared_ptr<LoxFunction> initializer = find_method("init");
    if (initializer) {
      return initializer->arity();
    }

    return 0;
  }

  std::string LoxClass::to_string() const {
    return m_name;
  }

  std::shared_ptr<LoxFunction> LoxClass::find_method(const std::string& name) {
    if (m_methods.count(name) > 0) {
      return m_methods[name];
    }

    if (m_superclass) {
      return m_superclass->find_method(name);
    }

    return nullptr;
  }

}
