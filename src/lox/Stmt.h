#ifndef STMT_H
#define STMT_H

namespace lox {

struct StmtVisitorString;
struct StmtVisitorVoid;

struct Stmt {
  virtual ~Stmt(){}
  virtual std::string accept(StmtVisitorString& visitor) = 0;
  virtual void accept(StmtVisitorVoid& visitor) = 0;
};

struct Expression;
struct Print;

struct StmtVisitorString {
  virtual std::string visit(Expression& stmt) = 0;
  virtual std::string visit(Print& stmt) = 0;
};

struct StmtVisitorVoid {
  virtual void visit(Expression& stmt) = 0;
  virtual void visit(Print& stmt) = 0;
};

struct Expression: public Stmt {
  public:
    Expression(std::unique_ptr<Expr> expr): expr(std::move(expr)) {}
    ~Expression() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    std::unique_ptr<Expr> expr;
};

struct Print: public Stmt {
  public:
    Print(std::unique_ptr<Expr> expr): expr(std::move(expr)) {}
    ~Print() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    std::unique_ptr<Expr> expr;
};

}

#endif //STMT_H
