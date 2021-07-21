#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"
#include "LoxReturn.hpp"
//#include "LoxInstance.hpp"
class LoxInstance;

namespace lox {

  class LoxFunction: public Callable {
    public:
      LoxFunction(Function& declaration, std::shared_ptr<Environment> closure, bool is_initializer);
      ~LoxFunction() {}
      virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override;
      virtual int arity() override;
      virtual std::string to_string() const override;
      std::shared_ptr<LoxFunction> bind(std::shared_ptr<Object> instance);
    private:
      Function m_declaration;
      std::shared_ptr<Environment> m_closure;
      bool m_is_initializer;
  };


}


#endif //FUNCTION_H
