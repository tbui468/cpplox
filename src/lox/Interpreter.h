#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include "Object.h"
#include "Expr.h"
#include "Stmt.h"
#include "RuntimeError.h"
#include "Environment.h"

namespace lox {

  class Interpreter: public VisitorObject, public StmtVisitorVoid {
    public:
      void interpret(const std::vector<std::unique_ptr<Stmt>>& statements);
    private:
      std::string stringify(const Object& obj);
      Object evaluate(Expr& expr);
      void execute(Stmt& stmt);
      void execute_block(const std::vector<std::unique_ptr<Stmt>>& statements, Environment& env);
      Object visit(Assign& expr) override;
      Object visit(Literal& expr) override;
      Object visit(Grouping& expr) override;
      Object visit(Unary& expr) override;
      Object visit(Binary& expr) override;
      Object visit(Variable& expr) override;
      void visit(Expression& stmt) override;
      void visit(Print& stmt) override;
      void visit(Var& stmt) override;
      void visit(Block& stmt) override;
      void visit(If& stmt) override;
      bool is_equal(Object a, Object b);
      void check_number_operand(Token op, Object operand);
      void check_number_operand(Token op, Object left, Object right);
    private:
      Environment m_environment;
  };

}


#endif //INTERPRETER_H
