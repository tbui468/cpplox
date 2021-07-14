#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Object.h"
#include "Expr.hpp"
#include "RuntimeError.h"

namespace lox {

  class Interpreter: public VisitorObject {
    public:
      Object evaluate(Expr& expr) {
        return expr.accept(*this);
      }
      Object visit(Literal& expr) override {
        return expr.value;
      }
      Object visit(Grouping& expr) override {
        return evaluate(*(expr.expr));
      }
      Object visit(Unary& expr) override {
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

      }

      Object visit(Binary& expr) override {
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
      }

      bool is_equal(Object a, Object b) {
        if (a.is_nil() && b.is_nil()) return true;
        if (a.is_nil()) return false;

        if (a.is_number() && b.is_number()) {
          return abs(a.get_number() - b.get_number()) < 0.001;
        } else if (a.is_string() && b.is_string()) {
          return a.get_string() == b.get_string();
        } else if (a.is_bool() && b.is_bool()) {
          return a.get_bool() == b.get_bool();
        }
      }

      void check_number_operand(Token op, Object operand) {
        if (operand.is_number()) return;
        throw RuntimeError(op, "Operand must be a number.");
      }

      void check_number_operand(Token op, Object left, Object right) {
        if (left.is_number() && right.is_number()) return;
        throw RuntimeError(op, "Operands must be numbers."); 
      }

  };

}


#endif //INTERPRETER_H
