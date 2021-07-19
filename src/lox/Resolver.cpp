#include "Resolver.hpp"


namespace lox {


  void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>>& stmts) {
    for (std::shared_ptr<Stmt> stmt: stmts) {
      resolve(stmt);
    }
  }

  /*
   * Expression overrides
   */

  void Resolver::visit(Variable& expr) {
    if (!m_scopes.empty() && m_scopes.back()[expr.name.m_lexeme] == false) {
      Lox::error(expr.name, "Can't read local variable in its own initializer.");
    }
    resolve_local(expr, expr.name);
  }

  void Resolver::visit(Assign& expr) {
    resolve(expr.value);
    resolve_local(expr, expr.name);
  }


  void Resolver::visit(Binary& expr) {
    resolve(expr.left);
    resolve(expr.right);
  }

  void Resolver::visit(Call& expr) {
    resolve(expr.callee);
    for (std::shared_ptr<Expr> a: expr.arguments) {
      resolve(expr);
    }
  }

  void Resolver::visit(Grouping& expr) {
    resolve(expr.expr);
  }

  void Resolver::visit(Literal& expr) {
    //resolver doesn't need to care about experssions
    //without subexpressions or variables
    return;
  }

  void Resolver::visit(Logical& expr) {
    resolve(expr.left);
    resolve(expr.right);
  }

  void Resolver::visit(Unary& expr) {
    resolve(expr.right);
  }


  /*
   * Statement overrides
   */

  void Resolver::visit(Function& stmt) {
    //declare and define function immediately since
    //we want to be able to use the function itself inside the body
    //can declare and define(set map value to true) parameters
    //immediately since paramater variables
    //will be set to arguments when the function is called
    declare(stmt.name);
    define(stmt.name);

    resolve_function(stmt);
  }

  void Resolver::visit(Block& stmt) {
    begin_scope();
    resolve(stmt.statements);
    end_scope();
  }

  //error is reported if the variable is used in initializer
  //this happens between declare() and define()
  //see visit(Variable& expr)
  void Resolver::visit(Var& stmt) {
    declare(stmt.name);
    if (!stmt.initializer) {
      resolve(stmt.initializer);
    }
    define(stmt.name);
  }

  void Resolver::visit(Expression& stmt) {
    resolve(stmt.expr);
  }

  void Resolver::visit(If& stmt) {
    resolve(stmt.condition);
    resolve(stmt.then_branch);
    if(!stmt.else_branch) {
      resolve(stmt.else_branch);
    }
  }


  void Resolver::visit(Print& stmt) {
    resolve(stmt.expr);
  }

  void Resolver::visit(Return& stmt) {
    if(stmt.value) {
      resolve(stmt.value);
    }
  }

  void Resolver::visit(While& stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);
  }

  /*
   * Override helper functions
   */

  void resolve_local(const Expr& expr, const Token& name) {
    if (m_scopes.empty()) return;
    for (int depth = 0; depth < m_scopes.size(); depth++) {
      if(m_scopes.at(m_scopes.size() - depth - 1).count(name.m_lexeme) > 0) {
        m_interpreter->resolve(expr, depth);
        return;
      }
    }
  }

  void Resolver::resolve_function(const Function& func) {
    begin_scope();
    for (const Token& param: func.params) {
      declare(token);
      define(token);
    }
    resolve(func.body);
    end_scope();
  }

  void Resolver::declare(const Token& name) {
    if (m_scopes.empty()) return;

    m_scopes.back()[name.m_lexeme] = false;
  }

  void Resolver::define(const Token& name) {
    if (m_scopes.empty()) return;

    m_scopes.back()[name.m_lexeme] = true;
  }

  void Resolver::begin_scope() {
    m_scopes.emplace(std::unordered_map<std::string, bool>);
  }

  void Resolver::end_scope() {
    m_scopes.pop();
  }


  void Resolver::resolve(std::shared_ptr<Stmt> stmt) {
    stmt->accept(*this);
  }

  void Resolver::resolve(std::shared_ptr<Expr> expr) {
    expr->accept(*this);
  }

}

