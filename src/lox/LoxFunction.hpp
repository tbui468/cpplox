#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"
#include "LoxReturn.hpp"

namespace lox {

  //class Function: public Callable, public Object {
  class LoxFunction: public Callable {
    public:
      LoxFunction(Function& declaration): Callable(), m_declaration(declaration) {}
      ~LoxFunction() {}

      virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override {
        std::shared_ptr<Environment> env = std::make_shared<Environment>(interp.m_globals);
        for (int i = 0; i < m_declaration.params.size(); i++) {
          env->define(m_declaration.params.at(i).m_lexeme, arguments.at(i));
        } 

        try {
          interp.execute_block(m_declaration.body, env);
        } catch(LoxReturn& ret) {
          return ret.value;
        }

        //if there is no return statement in function block
        return std::make_shared<Object>();
      }

      virtual int arity() override {
        return m_declaration.params.size();
      }

      virtual std::string to_string() override {
        return "<fn " + m_declaration.name.m_lexeme + ">";
      }
    private:
      Function m_declaration;
  };

}


#endif //FUNCTION_H
