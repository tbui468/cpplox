#ifndef CLASS_H
#define CLASS_H

#include <memory>
#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"

namespace lox {

  class LoxClass: public Callable, public std::enable_shared_from_this<LoxClass> {
    public:
      LoxClass(const std::string& name);
      ~LoxClass() {}
      virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override;
      virtual int arity() const override;
      std::string to_string() const override;
    private:
      std::string m_name;
  };


}


#endif //CLASS_H
