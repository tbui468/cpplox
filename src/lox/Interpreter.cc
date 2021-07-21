#include <iostream>
#include <chrono>
#include <unordered_map>
#include "Interpreter.h"
#include "Lox.h"
#include "LoxFunction.hpp"
#include "LoxReturn.hpp"
#include "LoxClass.hpp"
#include "LoxInstance.hpp"


namespace lox {

  Interpreter::Interpreter() {
    m_environment = std::make_shared<Environment>(); //m_environment references different environments when changing scope
    m_globals = m_environment; //but m_globals always points to the global environment (the one with m_enclosing = nullptr)
    class Clock: public Callable {
      public:
        Clock(): Callable() {}
        ~Clock() {}
        virtual std::shared_ptr<Object> call(Interpreter& interp, const std::vector<std::shared_ptr<Object>>& arguments) override {
          std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
          double seconds = std::chrono::duration<double>(time.time_since_epoch()).count();
          return std::make_shared<Object>(seconds);
        }
        virtual int arity() override {
          return 0;
        }
        virtual std::string to_string() const override {
          return "clock";
        }
    };
    m_globals->define("clock", std::make_shared<Clock>());
  }

  void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth) {
    m_locals[expr] = depth;
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


  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<Assign> expr) {
    std::shared_ptr<Object> value = evaluate(*(expr->value));

    if (m_locals.count(expr) > 0) {
      int dis = m_locals[expr];
      m_environment->assign_at(dis, expr->name, value);
    } else {
      m_globals->assign(expr->name, value);
    }
    return value;
  }

  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<Literal> expr) {
    return expr->value;
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

  std::shared_ptr<Object> Interpreter::look_up_variable(const Token& name, std::shared_ptr<Expr> expr) {
    if (m_locals.count(expr) > 0) {
      int dis = m_locals[expr];
      return m_environment->get_at(dis, name.m_lexeme);
    } else {
      return m_globals->get(name);
    }
  }

  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<Variable> expr) {
    return look_up_variable(expr->name, expr);
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


  std::shared_ptr<Object> Interpreter::visit(Call& expr) {
    //evaluate name of callee (normally just variable - function name)
    std::shared_ptr<Object> callee = evaluate(*(expr.callee));

    //Making sure callee is a function or class method
    if (!dynamic_cast<Callable*>(callee.get())) {
      throw RuntimeError(expr.paren, "Can only call functions and classes.");
    }

    //evaluate arguments
    std::vector<std::shared_ptr<Object>> arguments;
    for (std::shared_ptr<Expr> argument: expr.arguments) {
      arguments.push_back(evaluate(*argument));
    }

    //LoxFunction is a function or class method
    std::shared_ptr<LoxFunction> func = std::static_pointer_cast<LoxFunction>(callee); //<-- shouldn't this be dynamic pointer cast???

    if (static_cast<int>(arguments.size()) != func->arity()) {
      throw RuntimeError(expr.paren, "Expected " +
          std::to_string(func->arity()) + " arguments but got " +
          std::to_string(arguments.size()) + ".");
    }


    return func->call(*this, arguments);
  }

  //note: accessing a field looks like this: (expr->object).(expr->name)
  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<Get> expr) {
    std::shared_ptr<Object> obj = evaluate(*(expr->object));

    if (dynamic_cast<LoxInstance*>(obj.get())) {
      std::shared_ptr<LoxInstance> inst = std::dynamic_pointer_cast<LoxInstance>(obj);
      return inst->get(expr->name);
    }

    throw RuntimeError(expr->name, "Only instances have properties.");
  }

  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<Set> expr) {
    std::shared_ptr<Object> object = evaluate(*(expr->object));

    if(!dynamic_cast<LoxInstance*>(object.get())) {
      throw RuntimeError(expr->name, "Only instances have fields.");
    }

    std::shared_ptr<Object> value = evaluate(*(expr->value));
    std::dynamic_pointer_cast<LoxInstance>(object)->set(expr->name, value);
    return value;
  }

  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<This> expr) {
    return look_up_variable(expr->keyword, expr);
  }

  std::shared_ptr<Object> Interpreter::visit(std::shared_ptr<Super> expr) {
    //get the super class declare/defined in given scope in m_locals
    int dis = m_locals[expr];
    std::shared_ptr<LoxClass> superclass = std::dynamic_pointer_cast<LoxClass>(m_environment->get_at(dis, "super"));

    //get the instance of the super class
    std::shared_ptr<LoxInstance> object = std::dynamic_pointer_cast<LoxInstance>(m_environment->get_at(dis - 1, "this"));

    std::shared_ptr<LoxFunction> method = superclass->find_method(expr->method.m_lexeme);

    if (!method) {
      throw RuntimeError(expr->method, "Undefined property '" + expr->method.m_lexeme + "'.");
    }

    return method->bind(object);
  }

  /*
   * Statements
   */

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

  //instantiating LoxFunction with environment where the function is declared
  //Question: why can't two-stage variable binding be used here (like in Class)?
  //since functions should have a reference to itself from inside for recursion
  void Interpreter::visit(Function& stmt) {
    std::shared_ptr<Object> func = std::make_shared<LoxFunction>(stmt, m_environment, false);
    m_environment->define(stmt.name.m_lexeme, func);
  }

  void Interpreter::visit(Return& stmt) {
    if (stmt.value != nullptr) {
      std::shared_ptr<Object> value = evaluate(*stmt.value);
      throw LoxReturn(value);
    }
      
    throw LoxReturn(std::make_shared<Object>());  //return a nil object
  }

  //Note: two stage variable binding process 
  //allows references to the class from the methods inside
  void Interpreter::visit(std::shared_ptr<Class> stmt) {
    std::shared_ptr<Object> superclass = nullptr;
    if (stmt->superclass) {
      superclass = evaluate(*(stmt->superclass));
      if(!dynamic_cast<LoxClass*>(superclass.get())) {
        throw RuntimeError(stmt->superclass->name, "Superclass must be a class.");
      }
    }

    m_environment->define(stmt->name.m_lexeme, nullptr);

    //the closure of the methods contains "super", the superclass
    if (stmt->superclass) {
      m_environment = std::make_shared<Environment>(m_environment);
      m_environment->define("super", superclass);
    }

    //turn each class method into a LoxFunction object
    std::unordered_map<std::string, std::shared_ptr<LoxFunction>> methods;
    for (std::shared_ptr<Stmt> s: stmt->methods) {
      Function* m = dynamic_cast<Function*>(s.get());
      bool is_init = m->name.m_lexeme == "init";
      std::shared_ptr<LoxFunction> func = std::make_shared<LoxFunction>(*m, m_environment, is_init);
      methods[m->name.m_lexeme] = func;
    }

    std::shared_ptr<Object> klass = std::make_shared<LoxClass>(
                                      stmt->name.m_lexeme, 
                                      std::dynamic_pointer_cast<LoxClass>(superclass),
                                      methods);

    if (superclass) {
      m_environment = m_environment->m_enclosing;
    }

    m_environment->assign(stmt->name, klass);
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
