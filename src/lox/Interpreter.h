#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Object.h"
#include "Expr.hpp"

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
            return Object(left.get_number() - right.get_number());
          case TokenType::STAR:
            return Object(left.get_number() * right.get_number());
          case TokenType::SLASH:
            return Object(left.get_number() / right.get_number());
          case TokenType::PLUS:
            if (left.is_string() && right.is_string()) {
              return Object(left.get_string() + right.get_string());
            }else if(left.is_number() && right.is_number()) {
              return Object(left.get_number() + right.get_number());
            }
            break;
          case TokenType::GREATER:
            if (left.is_number() && right.is_number()) {
              return Object(left.get_number() > right.get_number());
            }
            break;
          case TokenType::GREATER_EQUAL:
            if (left.is_number() && right.is_number()) {
              return Object(left.get_number() >= right.get_number());
            }
            break;
          case TokenType::LESS:
            if (left.is_number() && right.is_number()) {
              return Object(left.get_number() < right.get_number());
            }
            break;
          case TokenType::LESS_EQUAL:
            if (left.is_number() && right.is_number()) {
              return Object(left.get_number() <= right.get_number());
            }
            break;
          case TokenType::BANG_EQUAL:
            return Object(!is_equal(left, right));
            break;
          case TokenType::EQUAL_EQUAL:
            return Object(is_equal(left, right));
            break;
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
  };

}


#endif //INTERPRETER_H
