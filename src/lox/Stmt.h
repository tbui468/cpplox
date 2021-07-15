#ifndef STMT_H
#define STMT_H

namespace lox {

struct Stmt {
  virtual ~Smt(){}
};

struct Expression: public Stmt;
struct Print: public Stmt;

struct StmtVisitorString {
  virtual std::string visit(Expression& stmt) = 0;
  virtual std::string visit(Print& stmt) = 0;
};

struct Expression: public Stmt {
  public:
    Expression(std::unique_ptr<Expr> expr): expr(expr) {}
    ~Expression() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
  public:
    std::unique_ptr<Expr> expr;
};

struct Print: public Stmt {
  public:
    Print(std::unique_ptr<Expr> expr): expr(expr) {}
    ~Print() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
  public:
    std::unique_ptr<Expr> expr;
};

}

#endif //STMT_H
