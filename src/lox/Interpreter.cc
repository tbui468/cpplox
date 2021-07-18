#include <iostream>
#include "Interpreter.h"
#include "Lox.h"
#include "LoxFunction.hpp"
#include "LoxReturn.hpp"


namespace lox {

  Interpreter::Interpreter() {
    m_environment = std::make_shared<Environment>(); //m_environment references different environments when changing scope
    m_globals = m_environment; //but m_globals always points to the global environment (the one with m_enclosing = nullptr)
    class Clock: public Callable {
      public:
        Clock(): Callable() {}
        ~Clock() {}
        virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override {
          return std::make_shared<Object>(1.0);
        }
        virtual int arity() const override {
          return 0;
        }
        virtual std::string to_string() const override {
          return "clock";
        }
    };
    m_globals->define("clock", std::make_shared<Clock>());
  }

  void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    try {
      for (const std::shared_ptr<Stmt>& statement: statements) {
        execute(*statement);
      }
    } catch (RuntimeError& error) {
      Lox::runtime_error(error);
    }
  }


  std::shared_ptr<Object> Interpreter::evaluate(Expr& expr) {
    return expr.accept(*this);
  }

  void Interpreter::execute(Stmt& stmt) {
    stmt.accept(*this);
  }

  void Interpreter::execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env) {
    std::shared_ptr<Environment> previous = m_environment; //need to save current m_environment state
    m_environment = env; //set m_environment to new state

    try {    
      for (const std::shared_ptr<Stmt>& stmt: statements) {
          execute(*stmt);
      }
    } catch(LoxReturn& ret) {
      m_environment = previous; //throw out environment and reset back to old one
      throw ret;
    }

    m_environment = previous; //throw out environment and reset back to old one

  }


  std::shared_ptr<Object> Interpreter::visit(Assign& expr) {
    std::shared_ptr<Object> value = evaluate(*(expr.value));
    m_environment->assign(expr.name, value);
    return value;
  }

  std::shared_ptr<Object> Interpreter::visit(Literal& expr) {
    return expr.value;
  }
  std::shared_ptr<Object> Interpreter::visit(Grouping& expr) {
    return evaluate(*(expr.expr));
  }
  std::shared_ptr<Object> Interpreter::visit(Unary& expr) {
    std::shared_ptr<Object> right = evaluate(*(expr.right));
    switch(expr.oprtr.m_type) {
      case TokenType::BANG:
        if (right->is_true()) return std::make_shared<Object>(false);
        else return std::make_shared<Object>(true);
      case TokenType::MINUS:
        check_number_operand(expr.oprtr, *right);
        right->set_number(-right->get_number());
        return right;
    }

    //here be dragons
    throw RuntimeError(expr.oprtr, "Error in unary expression.");

  }

  std::shared_ptr<Object> Interpreter::visit(Binary& expr) {
    std::shared_ptr<Object> left = evaluate(*(expr.left));
    std::shared_ptr<Object> right = evaluate(*(expr.right));

    switch(expr.oprtr.m_type) {
      case TokenType::MINUS:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() - right->get_number());
      case TokenType::STAR:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() * right->get_number());
      case TokenType::SLASH:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() / right->get_number());
      case TokenType::PLUS:
        if (left->is_string() && right->is_string()) {
          return std::make_shared<Object>(left->get_string() + right->get_string());
        }else if(left->is_number() && right->is_number()) {
          return std::make_shared<Object>(left->get_number() + right->get_number());
        }
        throw RuntimeError(expr.oprtr, "Operands must be two numbers or two strings");
        break;
      case TokenType::GREATER:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() > right->get_number());
      case TokenType::GREATER_EQUAL:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() >= right->get_number());
      case TokenType::LESS:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() < right->get_number());
      case TokenType::LESS_EQUAL:
        check_number_operand(expr.oprtr, *left, *right);
        return std::make_shared<Object>(left->get_number() <= right->get_number());
      case TokenType::BANG_EQUAL:
        return std::make_shared<Object>(!is_equal(*left, *right));
      case TokenType::EQUAL_EQUAL:
        return std::make_shared<Object>(is_equal(*left, *right));
    }        

    //here be dragons
    throw RuntimeError(expr.oprtr, "Error in binary expression.");
  }

  std::shared_ptr<Object> Interpreter::visit(Variable& expr) {
    return m_environment->get(expr.name);
  }

  std::shared_ptr<Object> Interpreter::visit(Logical& expr) {
    std::shared_ptr<Object> left = evaluate(*(expr.left));

    if (expr.oprtr.m_type == TokenType::OR) {
      if (left->is_true()) return left;
    } else {
      if (!left->is_true()) return left;
    }

    return evaluate(*(expr.right));
  }


  //This function requires massive rewrite or restructing to avoid raw pointer
  //are we computing the the arguments using 
  std::shared_ptr<Object> Interpreter::visit(Call& expr) {
    std::shared_ptr<Object> callee = evaluate(*(expr.callee));

    std::vector<std::shared_ptr<Object>> arguments;
    for (std::shared_ptr<Expr> argument: expr.arguments) {
      arguments.push_back(evaluate(*argument));
    }

    //NOTE: need to also check if class method (since they are also callable)
    if (!dynamic_cast<LoxFunction*>(callee.get())) {
      throw RuntimeError(expr.paren, "Can only call functions and classes.");
    }

    std::shared_ptr<LoxFunction> func = std::static_pointer_cast<LoxFunction>(callee);

    if (static_cast<int>(arguments.size()) != func->arity()) {
      throw RuntimeError(expr.paren, "Expected " +
          std::to_string(func->arity()) + " arguments but got " +
          std::to_string(arguments.size()) + ".");
    }

    return func->call(*this, arguments);
  }

  //statements
  void Interpreter::visit(Expression& stmt) {
    evaluate(*(stmt.expr)); //toss out result
  }

  void Interpreter::visit(Print& stmt) {
    std::shared_ptr<Object> value = evaluate(*(stmt.expr)); //<- this line is fucking up when the variable in undefined
    std::cout << stringify(*value) << std::endl;
  }

  void Interpreter::visit(Var& stmt) {
    std::shared_ptr<Object> value = std::make_shared<Object>(); //nil
    if (stmt.initializer != nullptr) {
      value = evaluate(*(stmt.initializer));
    }

    m_environment->define(stmt.name.m_lexeme, value);
  }
  
  void Interpreter::visit(Block& stmt) {
    std::shared_ptr<Environment> env = std::make_shared<Environment>(m_environment);
    execute_block(stmt.statements, env);
  }

  void Interpreter::visit(If& stmt) {
    std::shared_ptr<Object> condition = evaluate(*(stmt.condition));
    if (condition->is_true()) {
      execute(*(stmt.then_branch));      
    } else if(stmt.else_branch) {
      execute(*(stmt.else_branch));
    }
  }

  void Interpreter::visit(While& stmt) {
    while(evaluate(*stmt.condition)->is_true()) {
      execute(*(stmt.body));
    }
  }

  void Interpreter::visit(Function& stmt) {
    //instantiating LoxFunction with environment where the function is declared
    std::shared_ptr<Object> func = std::make_shared<LoxFunction>(stmt, m_environment);
    m_environment->define(stmt.name.m_lexeme, func);
  }

  void Interpreter::visit(Return& stmt) {
    if (stmt.value != nullptr) {
      std::shared_ptr<Object> value = evaluate(*stmt.value);
      throw LoxReturn(value);
    }
      
    throw LoxReturn(std::make_shared<Object>());  //return a nil object
  }

  bool Interpreter::is_equal(const Object& a, const Object& b) const {
    if (a.is_nil() && b.is_nil()) return true;
    if (a.is_nil()) return false;

    if (a.is_number() && b.is_number()) {
      return abs(a.get_number() - b.get_number()) < 0.001;
    } else if (a.is_string() && b.is_string()) {
      return a.get_string() == b.get_string();
    } else if (a.is_bool() && b.is_bool()) {
      return a.get_bool() == b.get_bool();
    }

    return false;
  }

  void Interpreter::check_number_operand(const Token& op, const Object& operand) const {
    if (operand.is_number()) return;
    throw RuntimeError(op, "Operand must be a number.");
  }

  void Interpreter::check_number_operand(const Token& op, const Object& left, const Object& right) const {
    if (left.is_number() && right.is_number()) return;
    throw RuntimeError(op, "Operands must be numbers."); 
  }


  std::string Interpreter::stringify(const Object& obj) const {
    if (obj.is_nil()) return "nil";

    if (obj.is_number()) {
      std::string num = std::to_string(obj.get_number());
      size_t pos = num.find(".");
      if (pos != std::string::npos && num.at(pos + 1) == '0') {
        return num.substr(0, pos);
      }
      return num;
    }

    if (obj.is_bool()) {
      if (obj.is_true()) return "true";
      else return "false";
    }

    //otherwise it's a string
    return obj.get_string();
  }

}
