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
      virtual ~Interpreter() {}
      void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
      void execute_block(const std::vector<std::shared_ptr<Stmt>>& statements, std::shared_ptr<Environment> env);
      void resolve(std::shared_ptr<Expr> expr, int depth);
    private:
      std::shared_ptr<Object> evaluate(Expr& expr);
      std::shared_ptr<Object> visit(std::shared_ptr<Assign> expr)   override;
      std::shared_ptr<Object> visit(std::shared_ptr<Literal> expr)  override;
      std::shared_ptr<Object> visit(Grouping& expr) override;
      std::shared_ptr<Object> visit(Unary& expr)    override;
      std::shared_ptr<Object> visit(Binary& expr)   override;
      std::shared_ptr<Object> visit(std::shared_ptr<Variable> expr) override;
      std::shared_ptr<Object> visit(Logical& expr)  override;
      std::shared_ptr<Object> visit(Call& expr)     override;
      std::shared_ptr<Object> visit(std::shared_ptr<Get> expr)     override;
      std::shared_ptr<Object> visit(std::shared_ptr<Set> expr)     override;
      std::shared_ptr<Object> look_up_variable(const Token& name, std::shared_ptr<Variable>);
      void execute(Stmt& stmt);
      void visit(Expression& stmt)  override;
      void visit(Print& stmt)       override;
      void visit(Var& stmt)         override;
      void visit(Block& stmt)       override;
      void visit(If& stmt)          override;
      void visit(While& stmt)       override;
      void visit(Function& stmt)    override;
      void visit(Return& stmt)      override;
      void visit(std::shared_ptr<Class> stmt) override;
      bool is_equal(const Object& a, const Object& b) const;
      void check_number_operand(const Token& op, const Object& operand) const;
      void check_number_operand(const Token& op, const Object& left, const Object& right) const;
      std::string stringify(const Object& obj) const;
    private:
      std::shared_ptr<Environment> m_environment;
      std::unordered_map<std::shared_ptr<Expr>, int> m_locals;
    public:
      std::shared_ptr<Environment> m_globals;
  };

}


#endif //INTERPRETER_H
