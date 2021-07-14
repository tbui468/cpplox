#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <vector>
#include <cstdarg>
#include "Expr.hpp"

namespace lox 
{

  class AstPrinter: public VisitorString {
    public:
      std::string print(Expr& expr);
      std::string visit(Binary& expr) override;
      std::string visit(Grouping& expr) override;
      std::string visit(Literal& expr) override;
      std::string visit(Unary& expr) override;
    private:
      std::string parenthesize(const std::string& name, const std::string& expr);
      std::string parenthesize(const std::string& name, const std::string& expr1, const std::string& expr2);
  };



}


#endif //AST_PRINTER_H
