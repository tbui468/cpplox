#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Object.h"
#include "Expr.hpp"
#include "RuntimeError.h"

namespace lox {

  class Interpreter: public VisitorObject {
    public:
      void interpret(Expr& expr);
    private:
      std::string stringify(const Object& obj);
      Object evaluate(Expr& expr);
      Object visit(Literal& expr) override;
      Object visit(Grouping& expr) override;
      Object visit(Unary& expr) override;
      Object visit(Binary& expr) override;
      bool is_equal(Object a, Object b);
      void check_number_operand(Token op, Object operand);
      void check_number_operand(Token op, Object left, Object right);
  };

}


#endif //INTERPRETER_H
