#include "AstPrinter.h"

namespace lox
{
  std::string AstPrinter::print(Expr& expr) {
    return expr.accept(*this);
  }

  std::string AstPrinter::print(Stmt& stmt) {
    return stmt.accept(*this);
  }

  std::string AstPrinter::visit(Binary& expr) {
    return parenthesize(expr.oprtr.m_lexeme, expr.left->accept(*this), expr.right->accept(*this));
  }

  std::string AstPrinter::visit(Grouping& expr) {
    return parenthesize("group", expr.expr->accept(*this));
  }

  std::string AstPrinter::visit(std::shared_ptr<Literal> expr) {
    if (expr->value->is_number()) return std::to_string(expr->value->get_number());
    if (expr->value->is_string()) return expr->value->get_string();
    if (expr->value->is_bool()) {
      if (expr->value->is_true()) return "true";
      else return "false";  
    }
    //must be nil
    return "nil";
  }

  std::string AstPrinter::visit(Unary& expr) {
    return parenthesize(expr.oprtr.m_lexeme, expr.right->accept(*this));
  }
  std::string AstPrinter::visit(std::shared_ptr<Assign> expr) {
    return "Assign";
  }
  std::string AstPrinter::visit(std::shared_ptr<Variable> expr) {
    return expr->name.m_lexeme;
  }
  std::string AstPrinter::visit(Logical& expr) {
    return "Logical";
  }
  std::string AstPrinter::visit(Call& expr) {
    return "Call";
  }

  std::string AstPrinter::visit(Expression& stmt) {
    return "Expression";
  }
  std::string AstPrinter::visit(Print& stmt) {
    return "Print: " + print(*(stmt.expr));
  }
  std::string AstPrinter::visit(Var& stmt) {
    return "Var";
  }
  std::string AstPrinter::visit(Block& stmt) {
    std::string out = ""; 
    for (const std::shared_ptr<Stmt>& stmt: stmt.statements) {
      out += print(*stmt) + "\n";
    }
    return out;
  }
  std::string AstPrinter::visit(If& stmt) {
    return "If";
  }
  std::string AstPrinter::visit(While& stmt) {
    return "While";
  }
  
  std::string AstPrinter::visit(Function& stmt) {
    std::string out = "Function:\n";
    //name Token, params vector<Token>, body vector<std::shared_ptr<Stmt>>
    std::string params = "";
    for (Token token: stmt.params) {
      params += token.to_string();
    }
    std::string body = "";
    for (const std::shared_ptr<Stmt>& s: stmt.body) {
      body += print(*s);
    }
    out += "  name: " + stmt.name.to_string() + "\n";
    out += "  params: " + params + "\n";
    out += "  body: " + body + "\n";
  
    return out;
  }

  std::string AstPrinter::visit(Return& stmt) {
    return "Return";
  }

  std::string AstPrinter::parenthesize(const std::string& name, const std::string& expr_string) {
    std::string out = "(" + name;
    out += " " + expr_string;
    out += ")";
    return out;
  }
  std::string AstPrinter::parenthesize(const std::string& name, const std::string& expr_string1, const std::string& expr_string2) {
    std::string out = "(" + name;
    out += " " + expr_string1;
    out += " " + expr_string2;
    out += ")";
    return out;
  }

}
