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
      Interpreter();
      void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
    private:
      std::string stringify(const Object& obj);
      std::shared_ptr<Object> evaluate(Expr& expr);
      std::shared_ptr<Object> visit(Assign& expr) override;
      std::shared_ptr<Object> visit(Literal& expr) override;
      std::shared_ptr<Object> visit(Grouping& expr) override;
      std::shared_ptr<Object> visit(Unary& expr) override;
      std::shared_ptr<Object> visit(Binary& expr) override;
      std::shared_ptr<Object> visit(Variable& expr) override;
      std::shared_ptr<Object> visit(Logical& expr) override;
      std::shared_ptr<Object> visit(Call& expr) override;
      void execute(Stmt& stmt);
      void visit(Expression& stmt) override;
      void visit(Print& stmt) override;
      void visit(Var& stmt) override;
      void visit(Block& stmt) override;
      void visit(If& stmt) override;
      void visit(While& stmt) override;
      void visit(Function& stmt) override;
      void visit(Return& stmt) override;
      bool is_equal(const Object& a, const Object& b);
      void check_number_operand(Token op, const Object& operand);
      void check_number_operand(Token op, const Object& left, const Object& right);
    private:
      std::shared_ptr<Environment> m_environment;
    public:
      std::shared_ptr<Environment> m_globals;
      void execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env);
  };

}


#endif //INTERPRETER_H
