#include "AstPrinter.h"

namespace lox
{
  std::string AstPrinter::print(Expr& expr) {
    return expr.accept(*this);
  }

  std::string AstPrinter::visit(Binary& expr) {
    return parenthesize(expr.oprtr.m_lexeme, expr.left->accept(*this), expr.right->accept(*this));
  }

  std::string AstPrinter::visit(Grouping& expr) {
    return parenthesize("group", expr.expr->accept(*this));
  }

  std::string AstPrinter::visit(Literal& expr) {
    if (expr.value.is_number()) return std::to_string(expr.value.get_number());
    if (expr.value.is_string()) return expr.value.get_string();
    if (expr.value.is_bool()) {
      if (expr.value.is_true()) return "true";
      else return "false";  
    }
    //must be nil
    return "nil";
  }

  std::string AstPrinter::visit(Unary& expr) {
    return parenthesize(expr.oprtr.m_lexeme, expr.right->accept(*this));
  }
  std::string AstPrinter::visit(Assign& expr) {
    return "Assign";
  }
  std::string AstPrinter::visit(Variable& expr) {
    return expr.name.m_lexeme;
  }
  std::string AstPrinter::visit(Logical& expr) {
    return "Logical";
  }
  std::string AstPrinter::visit(Call& expr) {
    return "Call";
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
