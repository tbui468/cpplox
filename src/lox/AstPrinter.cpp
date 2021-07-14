#include "AstPrinter.hpp"

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
    if (expr.value == "") return "nil";
    else return expr.value; //don't need to convert to Number since AstPrinter wants a string to print
  }

  std::string AstPrinter::visit(Unary& expr) {
    return parenthesize(expr.oprtr.m_lexeme, expr.right->accept(*this));
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
