#include "LoxFunction.hpp"

namespace lox {

  LoxFunction::LoxFunction(Function& declaration, std::shared_ptr<Environment> closure): 
    Callable(), m_declaration(declaration), m_closure(closure) {}

  std::shared_ptr<Object> LoxFunction::call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(m_closure);
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

  int LoxFunction::arity() const {
    return m_declaration.params.size();
  }

  std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<Object> instance) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(m_closure);
    env->define("this", instance);
    return std::make_shared<LoxFunction>(m_declaration, env);
  }

  std::string LoxFunction::to_string() const {
    return "<fn " + m_declaration.name.m_lexeme + ">";
  }

}
