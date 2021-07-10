#include "AstPrinter.hpp"

namespace lox
{

  std::string AstPrinter::visit(Binary& e)
  {
    return "binary";
  }
  std::string AstPrinter::visit(Grouping& e)
  {
    return "grouping";
  }
  std::string AstPrinter::visit(Literal& e)
  {
    if (e.value == "") return "nil";
    else return e.value;
  }
  std::string AstPrinter::visit(Unary& e)
  {
    return "unary";
  }
  std::string AstPrinter::parenthesize(const std::string& name, std::vector<Expr> exprs)
  {
    //what is name???  exprs is a list of expressions.  Name is "group" or the lexeme (what are the possible lexemes?)
  }

}
