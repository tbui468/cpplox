#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"
#include "LoxReturn.hpp"

namespace lox {

  class LoxFunction: public Callable {
    public:
      LoxFunction(Function& declaration, std::shared_ptr<Environment> closure);
      ~LoxFunction() {}
      virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override;
      virtual int arity() const override;
      virtual std::string to_string() const override;
    private:
      Function m_declaration;
      std::shared_ptr<Environment> m_closure;
  };


}


#endif //FUNCTION_H
