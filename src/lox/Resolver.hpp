#ifndef RESOLVER_H
#define RESOLVER_H


#include <memory>
#include <vector>
#include <unordered_map>
#include "Expr.h"
#include "Stmt.h"
#include "Object.h"
#include "Interpreter.h"
#include "Token.h"

namespace lox {

  class Resolver: public ExprVisitorVoid, public StmtVisitorVoid {
    public:
      Resolver(std::shared_ptr<Interpreter> interpreter): m_interpreter(interpreter) {}
      ~Resolver() {}
      void resolve(const std::vector<std::shared_ptr<Stmt>>& stmts);
    private:
      enum class FunctionType {
        NONE,
        FUNCTION,
        INITIALIZER,
        METHOD
      };
      enum class ClassType {
        NONE,
        CLASS
      };
      //expressions
      void visit(std::shared_ptr<Assign> expr)   override;
      void visit(std::shared_ptr<Literal> expr)  override;
      void visit(Grouping& expr) override;
      void visit(Unary& expr)    override;
      void visit(Binary& expr)   override;
      void visit(std::shared_ptr<Variable> expr) override;
      void visit(Logical& expr)  override;
      void visit(Call& expr)     override;
      void visit(std::shared_ptr<Get> expr) override;
      void visit(std::shared_ptr<Set> expr) override;
      void visit(std::shared_ptr<This> expr) override;
      //statments
      void visit(Expression& stmt)  override;
      void visit(Print& stmt)       override;
      void visit(Var& stmt)         override;
      void visit(Block& stmt)       override;
      void visit(If& stmt)          override;
      void visit(While& stmt)       override;
      void visit(Function& stmt)    override;
      void visit(Return& stmt)      override;
      void visit(std::shared_ptr<Class> stmt) override;
    private:
      void declare(const Token& name);
      void define(const Token& name);
      void begin_scope();
      void end_scope();
      void resolve(std::shared_ptr<Stmt> stmt);
      void resolve(std::shared_ptr<Expr> expr);
      void resolve_local(std::shared_ptr<Expr> expr, const Token& name);
      void resolve_function(const Function& func, FunctionType type);
    private:
      std::shared_ptr<Interpreter> m_interpreter {nullptr};
      std::vector<std::unordered_map<std::string, bool>> m_scopes;
      FunctionType m_current_function {FunctionType::NONE};
      ClassType m_current_class {ClassType::NONE};
  };

}

#endif //RESOLVER_H
