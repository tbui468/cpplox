#include <iostream>
#include "Interpreter.h"
#include "Lox.h"


namespace lox {

  void Interpreter::interpret(const std::vector<std::unique_ptr<Stmt>>& statements) {
    try {
      for (const std::unique_ptr<Stmt>& statement: statements) {
        execute(*statement);
      }
    } catch (RuntimeError& error) {
      Lox::runtime_error(error);
    }
  }

  //if first digit after decimal is 0, treat the return string as an integer
  //eg, chop off the decimal point and all digits after that
  std::string Interpreter::stringify(const Object& obj) {
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

  Object Interpreter::evaluate(Expr& expr) {
    return expr.accept(*this);
  }

  void Interpreter::execute(Stmt& stmt) {
    stmt.accept(*this);
  }

  Object Interpreter::visit(Literal& expr) {
    return expr.value;
  }
  Object Interpreter::visit(Grouping& expr) {
    return evaluate(*(expr.expr));
  }
  Object Interpreter::visit(Unary& expr) {
    Object right = evaluate(*(expr.right));
    switch(expr.oprtr.m_type) {
      case TokenType::BANG:
        if (right.is_true()) return Object(false);
        else return Object(true);
      case TokenType::MINUS:
        check_number_operand(expr.oprtr, right);
        right.set_number(-right.get_number());
        return right;
    }

    //here be dragons
    throw RuntimeError(expr.oprtr, "Error in unary expression.");

  }

  Object Interpreter::visit(Binary& expr) {
    Object left = evaluate(*(expr.left));
    Object right = evaluate(*(expr.right));

    switch(expr.oprtr.m_type) {
      case TokenType::MINUS:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() - right.get_number());
      case TokenType::STAR:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() * right.get_number());
      case TokenType::SLASH:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() / right.get_number());
      case TokenType::PLUS:
        if (left.is_string() && right.is_string()) {
          return Object(left.get_string() + right.get_string());
        }else if(left.is_number() && right.is_number()) {
          return Object(left.get_number() + right.get_number());
        }
        throw RuntimeError(expr.oprtr, "Operands must be two numbers or two strings");
      case TokenType::GREATER:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() > right.get_number());
      case TokenType::GREATER_EQUAL:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() >= right.get_number());
      case TokenType::LESS:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() < right.get_number());
      case TokenType::LESS_EQUAL:
        check_number_operand(expr.oprtr, left, right);
        return Object(left.get_number() <= right.get_number());
      case TokenType::BANG_EQUAL:
        return Object(!is_equal(left, right));
      case TokenType::EQUAL_EQUAL:
        return Object(is_equal(left, right));
    }        

    //here be dragons
    throw RuntimeError(expr.oprtr, "Error in binary expression.");
  }

  Object Interpreter::visit(Variable& expr) {
    return m_environment.get(expr.name);
  }

  void Interpreter::visit(Expression& stmt) {
    evaluate(*(stmt.expr)); //toss out result
  }

  void Interpreter::visit(Print& stmt) {
    Object value = evaluate(*(stmt.expr));
    std::cout << stringify(value) << std::endl;
  }

  void Interpreter::visit(Var& stmt) {
    Object value = Object(); //nil
    if (stmt.initializer != nullptr) {
      value = evaluate(*(stmt.initializer));
    }

    m_environment.define(stmt.name.m_lexeme, value);
  }

  bool Interpreter::is_equal(Object a, Object b) {
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

  void Interpreter::check_number_operand(Token op, Object operand) {
    if (operand.is_number()) return;
    throw RuntimeError(op, "Operand must be a number.");
  }

  void Interpreter::check_number_operand(Token op, Object left, Object right) {
    if (left.is_number() && right.is_number()) return;
    throw RuntimeError(op, "Operands must be numbers."); 
  }

}
