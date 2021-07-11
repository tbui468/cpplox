#include "AstPrinter.hpp"

namespace lox
{
  std::string AstPrinter::print(Expr& expr) 
  {
    return expr.accept(*this);
  }

  std::string AstPrinter::visit(Binary& expr)
  {
    std::vector<Expr*> exprs = {expr.left, expr.right};
    return parenthesize(expr.oprtr->m_lexeme, exprs);
  }
  std::string AstPrinter::visit(Grouping& expr)
  {
    std::vector<Expr*> exprs = {expr.expr};
    return parenthesize("group", exprs);
  }
  std::string AstPrinter::visit(Literal& expr)
  {
    if (expr.value == "") return "nil";
    else return expr.value; //don't need to convert to Number since AstPrinter wants a string to print
  }
  std::string AstPrinter::visit(Unary& expr)
  {
    std::vector<Expr*> exprs = {expr.right};
    return parenthesize(expr.oprtr->m_lexeme, exprs);
  }
  std::string AstPrinter::parenthesize(const std::string& name, std::vector<Expr*> exprs)
  {
    std::string out = "(" + name;
    for (Expr* e: exprs) {
      out += " ";
      out += e->accept(*this);
    }
    out += ")";
    return out;
  }

}
