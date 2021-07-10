#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "Expr.hpp"

namespace lox 
{

  class AstPrinter: public VisitorInterface<std::string> {
    public:
      std::string visit(Binary& e) override;
      std::string visit(Grouping& e) override;
      std::string visit(Literal& e) override;
      std::string visit(Unary& e) override;
    private:
      std::string parenthesize(const std::string& name, std::vector<Expr> exprs);
  };



}


#endif //AST_PRINTER_H
