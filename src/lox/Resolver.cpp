#include "Resolver.hpp"
#include "Lox.h"


namespace lox {


  void Resolver::resolve(const std::vector<std::shared_ptr<Stmt>>& stmts) {
    for (std::shared_ptr<Stmt> stmt: stmts) {
      resolve(stmt);
    }
  }

  void Resolver::resolve(std::shared_ptr<Stmt> stmt) {
    stmt->accept(*this);
  }

  void Resolver::resolve(std::shared_ptr<Expr> expr) {
    expr->accept(*this);
  }

  /*
   * Expression overrides
   */

  //this is getting called on valid inputs
  void Resolver::visit(std::shared_ptr<Variable> expr) {
    if (!m_scopes.empty() && 
        m_scopes.back().count(expr->name.m_lexeme) > 0 && //is this necessary?  Check with compiler explorer
        m_scopes.back()[expr->name.m_lexeme] == false //if the variable being access was declared but not defined in current scope
        ) {
      Lox::error(expr->name, "Can't read local variable in its own initializer.");
    }
    resolve_local(expr, expr->name);
  }

  void Resolver::visit(std::shared_ptr<Assign> expr) {
    resolve(expr->value);
    resolve_local(expr, expr->name);
  }


  void Resolver::visit(Binary& expr) {
    resolve(expr.left);
    resolve(expr.right);
  }

  void Resolver::visit(Call& expr) {
    resolve(expr.callee);
    for (std::shared_ptr<Expr> a: expr.arguments) {
      resolve(a);
    }
  }

  void Resolver::visit(std::shared_ptr<Get> expr) {
    resolve(expr->object);
  }

  void Resolver::visit(std::shared_ptr<Set> expr) {
    resolve(expr->value);
    resolve(expr->object);
  }

  void Resolver::visit(Grouping& expr) {
    resolve(expr.expr);
  }

  void Resolver::visit(std::shared_ptr<Literal> expr) {
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

  void Resolver::visit(std::shared_ptr<This> expr) {
    if (m_current_class == ClassType::NONE) {
      Lox::error(expr->keyword, "Can't use 'this' outside of a class.");
    }
    resolve_local(expr,expr->keyword);
  }

  void Resolver::visit(std::shared_ptr<Super> expr) {
    if (m_current_class == ClassType::NONE) {
      Lox::error(expr->keyword, "Can't use 'super' outside of a class.");
    } else if (m_current_class != ClassType::SUBCLASS) {
      Lox::error(expr->keyword, "Can't use 'super' in a class with no superclass.");
    }

    resolve_local(expr, expr->keyword);
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

    resolve_function(stmt, FunctionType::FUNCTION);
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
    if (stmt.initializer) {
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
    if(stmt.else_branch) {
      resolve(stmt.else_branch);
    }
  }


  void Resolver::visit(Print& stmt) {
    resolve(stmt.expr);
  }

  void Resolver::visit(Return& stmt) {
    if (m_current_function == FunctionType::NONE) {
      Lox::error(stmt.keyword, "Can't return from top-level code.");
    }
    if(stmt.value) {
      if (m_current_function == FunctionType::INITIALIZER) {
        Lox::error(stmt.keyword, "Can't return a value from initializer.");
      }
      resolve(stmt.value);
    }
  }

  void Resolver::visit(While& stmt) {
    resolve(stmt.condition);
    resolve(stmt.body);
  }

  void Resolver::visit(std::shared_ptr<Class> stmt) {
    ClassType enclosing_class = m_current_class;
    m_current_class = ClassType::CLASS;

    declare(stmt->name);
    define(stmt->name);

    if (stmt->superclass && stmt->name.m_lexeme == stmt->superclass->name.m_lexeme) {
      Lox::error(stmt->superclass->name, "A class can't inherit from itself.");
    }

    if (stmt->superclass) {
      m_current_class = ClassType::SUBCLASS;
      resolve(stmt->superclass);
    }

    if (stmt->superclass) {
      begin_scope();
      m_scopes.back()["super"] = true;
    }

    begin_scope();
    m_scopes.back()["this"] = true;

    for (std::shared_ptr<Stmt> s: stmt->methods) {
      Function* method = dynamic_cast<Function*>(s.get());
      FunctionType declaration = FunctionType::METHOD;
      if (method->name.m_lexeme == "init") {
        declaration = FunctionType::INITIALIZER;
      }
      resolve_function(*method, declaration);
    }

    end_scope();

    if (stmt->superclass) {
      end_scope();
    }

    m_current_class = enclosing_class;
  }


  /*
   * Override helper functions
   */

  //resolve_local is the only function to add key/values to the interperter locals table
  //calling resolve on the interpreter adds a pointer to the expression + scope depth
  //where a depth of 0 means current scope.
  void Resolver::resolve_local(std::shared_ptr<Expr> expr, const Token& name) {
    if (m_scopes.empty()) return;
    for (int depth = 0; depth < static_cast<int>(m_scopes.size()); depth++) {
      if(m_scopes.at(m_scopes.size() - depth - 1).count(name.m_lexeme) > 0) {
        m_interpreter->resolve(expr, depth);
        return;
      }
    }
  }

  void Resolver::resolve_function(const Function& func, FunctionType type) {
    FunctionType enclosing_function = m_current_function; //FunctionType logic is for preventing 'return' statements in global scope
    m_current_function = type;
    begin_scope();
    for (const Token& param: func.params) {
      declare(param);
      define(param);
    }
    resolve(func.body);
    end_scope();
    m_current_function = enclosing_function;
  }

  void Resolver::declare(const Token& name) {
    if (m_scopes.empty()) return;
    if (m_scopes.back().count(name.m_lexeme) > 0) {
      Lox::error(name, "Already a variable with this name declared in this scope.");
    }
    m_scopes.back()[name.m_lexeme] = false;
  }

  void Resolver::define(const Token& name) {
    if (m_scopes.empty()) return;

    m_scopes.back()[name.m_lexeme] = true;
  }

  void Resolver::begin_scope() {
    m_scopes.emplace_back(std::unordered_map<std::string, bool>());
  }

  void Resolver::end_scope() {
    m_scopes.pop_back();
  }



}

