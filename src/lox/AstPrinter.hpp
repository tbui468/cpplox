#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <vector>
#include "Expr.hpp"

namespace lox 
{

  class AstPrinter: public VisitorInterfaceString {
    public:
      std::string print(Expr& expr);
      std::string visit(Binary& expr) override;
      std::string visit(Grouping& expr) override;
      std::string visit(Literal& expr) override;
      std::string visit(Unary& expr) override;
    private:
      std::string parenthesize(const std::string& name, std::vector<Expr*> exprs);
  };



}


#endif //AST_PRINTER_H
