#ifndef CLASS_H
#define CLASS_H

#include <memory>
#include <unordered_map>
#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"
#include "LoxFunction.hpp"

namespace lox {

  class LoxClass: public Callable, public std::enable_shared_from_this<LoxClass> {
    public:
      LoxClass(const std::string& name, 
               std::shared_ptr<LoxClass> superclass,
               std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods);
      ~LoxClass() {}
      virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override;
      virtual int arity() override;
      std::string to_string() const override;
      std::shared_ptr<LoxFunction> find_method(const std::string& name);
    private:
      std::string m_name;
      std::shared_ptr<LoxClass> m_superclass;
      std::unordered_map<std::string, std::shared_ptr<LoxFunction>> m_methods;
  };


}


#endif //CLASS_H
