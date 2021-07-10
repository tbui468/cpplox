#ifndef EXPR_H
#define EXPR_H

#include "Token.hpp"
namespace lox {

struct Expr {
};

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

template <typename T>
struct VisitorInterface {
  virtual T visit(Binary& e) = 0;
  virtual T visit(Grouping& e) = 0;
  virtual T visit(Literal& e) = 0;
  virtual T visit(Unary& e) = 0;
};

struct Binary: public Expr {
  public:
    Binary(Expr left, Token oprtr, Expr right): left(left), oprtr(oprtr), right(right) {}
    template <typename T>
    T accept(VisitorInterface<T>& v) {
      return v.visit(*this); 
    }
  public:
    Expr left;
    Token oprtr;
    Expr right;
};

struct Grouping: public Expr {
  public:
    Grouping(Expr expression): expression(expression) {}
    template <typename T>
    T accept(VisitorInterface<T>& v) {
      return v.visit(*this); 
    }
  public:
    Expr expression;
};

struct Literal: public Expr {
  public:
    Literal(std::string value): value(value) {}
    template <typename T>
    T accept(VisitorInterface<T>& v) {
      return v.visit(*this); 
    }
  public:
    std::string value;
};

struct Unary: public Expr {
  public:
    Unary(Token oprtr, Expr right): oprtr(oprtr), right(right) {}
    template <typename T>
    T accept(VisitorInterface<T>& v) {
      return v.visit(*this); 
    }
  public:
    Token oprtr;
    Expr right;
};

//implement version of this where T is std::string
struct VisitorString: public VisitorInterface<std::string> {
  std::string visit(Binary& e) override {
    return "I'm the binary";
  }
  std::string visit(Grouping& e) override {
    return "I'm the grouping";
  }
  std::string visit(Literal& e) override {
    return "I'm the literal";
  }
  std::string visit(Unary& e) override {
    return "I'm the unary";
  }
};

}

#endif //EXPR_H
