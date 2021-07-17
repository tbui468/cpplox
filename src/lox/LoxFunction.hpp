#ifndef FUNCTION_H
#define FUNCTION_H

#include "Callable.hpp"
#include "Stmt.h"
#include "Environment.h"

namespace lox {

  //class Function: public Callable, public Object {
  class LoxFunction: public Callable {
    public:
      //LoxFunction(const Function& declaration): Callable(), m_declaration(declaration) {}
      LoxFunction(): Callable() {}

      ~LoxFunction() {}

      virtual Object call(Interpreter& interp, const std::vector<Object>& arguments) override {
        /*
        std::shared_ptr<Environment> env = std::make_shared<Environment>(interp.m_globals);
        for (int i = 0; i < m_declaration.params.size(); i++) {
          env->define(m_declaration.params.at(i).m_lexeme, arguments.at(i));
        } 

        interp.execute_block(m_declaration.body, env);*/
        return Object(); //note: 0 arguments is a null Object
      }

      virtual int arity() override {
        //return m_declaration.params.size();
        return 0;
      }

      virtual std::string to_string() override {
        //return "<fn " + m_declaration.name.m_lexeme + ">";
        return "nothing";
      }
    private:
      //Function m_declaration;
  };

}


#endif //FUNCTION_H
