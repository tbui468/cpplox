#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include "Token.h"
#include "Object.h"

namespace lox {

struct VisitorString;
struct VisitorObject;

struct Expr {
  virtual ~Expr(){}
  virtual std::string accept(VisitorString& v) = 0;
  virtual Object accept(VisitorObject& v) = 0;
};

struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;
struct Logical;
struct Call;

struct VisitorString {
  virtual std::string visit(Assign& e) = 0;
  virtual std::string visit(Binary& e) = 0;
  virtual std::string visit(Grouping& e) = 0;
  virtual std::string visit(Literal& e) = 0;
  virtual std::string visit(Unary& e) = 0;
  virtual std::string visit(Variable& e) = 0;
  virtual std::string visit(Logical& e) = 0;
  virtual std::string visit(Call& e) = 0;
};

struct VisitorObject {
  virtual Object visit(Assign& e) = 0;
  virtual Object visit(Binary& e) = 0;
  virtual Object visit(Grouping& e) = 0;
  virtual Object visit(Literal& e) = 0;
  virtual Object visit(Unary& e) = 0;
  virtual Object visit(Variable& e) = 0;
  virtual Object visit(Logical& e) = 0;
  virtual Object visit(Call& e) = 0;
};


struct Assign: public Expr {
  public:
    Assign(Token name, std::shared_ptr<Expr> value): name(name), value(value) {}
    ~Assign() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    Token name;
    std::shared_ptr<Expr> value;
};

struct Binary: public Expr {
  public:
    Binary(Token oprtr, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right): oprtr(oprtr), left(left), right(right) {}
    ~Binary() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    Token oprtr;
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
};

//this is really just the base Expr
struct Grouping: public Expr {
  public:
    Grouping(std::shared_ptr<Expr> expr): expr(expr) {}
    ~Grouping() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    std::shared_ptr<Expr> expr;
};

struct Literal: public Expr {
  public:
    Literal(Object value): value(value) {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    Object value;
};

struct Unary: public Expr {
  public:
    Unary(Token oprtr, std::shared_ptr<Expr> right): oprtr(oprtr), right(right) {}
    ~Unary() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    Token oprtr;
    std::shared_ptr<Expr> right;
};

struct Variable: public Expr {
  public:
    Variable(Token name): name(name) {}
    ~Variable() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    Token name;
};

struct Logical: public Expr {
  public:
    Logical(Token oprtr, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right):
      oprtr(oprtr), left(left), right(right) {}
    ~Logical() {}
    std::string accept(VisitorString& visitor) { return visitor.visit(*this); }
    Object accept(VisitorObject& visitor) { return visitor.visit(*this); }
  public:
    Token oprtr;
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
};

struct Call: Expr {
  public:
    Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments):
      callee(callee), paren(paren), arguments(arguments) {}
    ~Call() {}
    std::string accept(VisitorString& visitor) { return visitor.visit(*this); }
    Object accept(VisitorObject& visitor) { return visitor.visit(*this); }
  public:
    std::shared_ptr<Expr> callee; //any expression that evaluates to a function
    Token paren; //keeping closing token for runtime error reporting
    std::vector<std::shared_ptr<Expr>> arguments;
};

}

#endif //EXPR_H
