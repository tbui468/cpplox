#include "LoxFunction.hpp"

namespace lox {

  LoxFunction::LoxFunction(Function& declaration, std::shared_ptr<Environment> closure, bool is_initializer): 
    Callable(), m_declaration(declaration), m_closure(closure), m_is_initializer(is_initializer) {}

  std::shared_ptr<Object> LoxFunction::call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(m_closure);
    for (int i = 0; i < m_declaration.params.size(); i++) {
      env->define(m_declaration.params.at(i).m_lexeme, arguments.at(i));
    } 

    try {
      interp.execute_block(m_declaration.body, env);
    } catch(LoxReturn& ret) {
      if (m_is_initializer) {
        return m_closure->get_at(0, "this");
      }
      return ret.value;
    }

    if (m_is_initializer) {
      return m_closure->get_at(0, "this");
    }

    //if there is no return statement in function block, return lox::null
    return std::make_shared<Object>();
  }

  int LoxFunction::arity() {
    return m_declaration.params.size();
  }

  std::shared_ptr<LoxFunction> LoxFunction::bind(std::shared_ptr<Object> instance) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(m_closure);
    env->define("this", instance);
    return std::make_shared<LoxFunction>(m_declaration, env, m_is_initializer);
  }

  std::string LoxFunction::to_string() const {
    return "<fn " + m_declaration.name.m_lexeme + ">";
  }

}
