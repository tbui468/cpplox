#ifndef EXPR_H
#define EXPR_H
#include "Token.hpp"
namespace lox {

struct Expr {
};



struct Binary: public Expr {
  Binary(Expr left, Token oprtr, Expr right): left(left), oprtr(oprtr), right(right) {}
  Expr left;
  Token oprtr;
  Expr right;
};


struct Grouping: public Expr {
  Grouping(Expr expression): expression(expression) {}
  Expr expression;
};


struct Literal: public Expr {
  Literal(std::string value): value(value) {}
  std::string value;
};


struct Unary: public Expr {
  Unary(Token oprtr, Expr right): oprtr(oprtr), right(right) {}
  Token oprtr;
  Expr right;
};
}

#endif //EXPR_H
