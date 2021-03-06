#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <vector>
#include <cstdarg>
#include "Expr.h"
#include "Stmt.h"

namespace lox 
{

  class AstPrinter: public VisitorString, public StmtVisitorString {
    public:
      std::string print(Expr& expr);
      std::string print(Stmt& stmt);
    private:
      std::string visit(Binary& expr)     override;
      std::string visit(Grouping& expr)   override;
      std::string visit(std::shared_ptr<Literal> expr)    override;
      std::string visit(Unary& expr)      override;
      std::string visit(std::shared_ptr<Assign> expr)     override;
      std::string visit(std::shared_ptr<Variable> expr)   override;
      std::string visit(Logical& expr)    override;
      std::string visit(Call& expr)       override;
      std::string visit(std::shared_ptr<Get> expr)   override;
      std::string visit(std::shared_ptr<Set> expr)   override;
      std::string visit(std::shared_ptr<This> expr)   override;
      std::string visit(std::shared_ptr<Super> expr)   override;
    private:
      std::string visit(Expression& stmt) override;
      std::string visit(Print& stmt)      override;
      std::string visit(Var& stmt)        override;
      std::string visit(Block& stmt)      override;
      std::string visit(If& stmt)         override;
      std::string visit(While& stmt)      override;
      std::string visit(Function& stmt)   override;
      std::string visit(Return& stmt)     override;
      std::string visit(std::shared_ptr<Class> stmt) override;
    private:
      std::string parenthesize(const std::string& name, const std::string& expr);
      std::string parenthesize(const std::string& name, const std::string& expr1, const std::string& expr2);
  };



}


#endif //AST_PRINTER_H
