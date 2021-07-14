#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include "Token.hpp"
#include "Object.h"

namespace lox {

struct VisitorString;
struct VisitorObject;

struct Expr {
  virtual ~Expr(){}
  virtual std::string accept(VisitorString& v) = 0;
  virtual Object accept(VisitorObject& v) = 0;
};

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct VisitorString {
  virtual std::string visit(Binary& e) = 0;
  virtual std::string visit(Grouping& e) = 0;
  virtual std::string visit(Literal& e) = 0;
  virtual std::string visit(Unary& e) = 0;
};

struct VisitorObject {
  virtual Object visit(Binary& e) = 0;
  virtual Object visit(Grouping& e) = 0;
  virtual Object visit(Literal& e) = 0;
  virtual Object visit(Unary& e) = 0;
};

struct Binary: public Expr {
  public:
    Binary(Token oprtr, std::unique_ptr<Expr> left, std::unique_ptr<Expr> right): oprtr(oprtr), left(std::move(left)), right(std::move(right)) {}
    ~Binary() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
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
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    std::unique_ptr<Expr> expr;
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
    Unary(Token oprtr, std::unique_ptr<Expr> right): oprtr(oprtr), right(std::move(right)) {}
    ~Unary() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    Object accept(VisitorObject& v) { return v.visit(*this); }
  public:
    Token oprtr; //note: Token has a type, lexeme, literal (optional) and line 
    std::unique_ptr<Expr> right;
};


}

#endif //EXPR_H
