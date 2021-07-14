#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include "Token.hpp"

namespace lox {

struct VisitorInterfaceString;

struct Expr {
  virtual ~Expr(){}
  virtual std::string accept(VisitorInterfaceString& v) = 0;
};

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct VisitorInterfaceString {
  virtual std::string visit(Binary& e) = 0;
  virtual std::string visit(Grouping& e) = 0;
  virtual std::string visit(Literal& e) = 0;
  virtual std::string visit(Unary& e) = 0;
};

struct Binary: public Expr {
  public:
    Binary(Token oprtr, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right): oprtr(oprtr), left(std::move(left)), right(std::move(right)) {}
    ~Binary() {}
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
  public:
    Token oprtr;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
};

//this is really just the base Expr
struct Grouping: public Expr {
  public:
    Grouping(std::unique_ptr<Expr> expr): expr(std::move(expr)) {}
    ~Grouping() {}
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
  public:
    std::unique_ptr<Expr> expr;
};

struct Literal: public Expr {
  public:
    Literal(const std::string& value): value(value) {}
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
  public:
    std::string value;
};

struct Unary: public Expr {
  public:
    Unary(Token oprtr, std::unique_ptr<Expr> right): oprtr(oprtr), right(std::move(right)) {}
    ~Unary() {}
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
  public:
    Token oprtr; //note: Token has a type, lexeme, literal (optional) and line 
    std::unique_ptr<Expr> right;
};


}

#endif //EXPR_H
