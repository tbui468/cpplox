#ifndef EXPR_H
#define EXPR_H

#include "Token.hpp"
namespace lox {

struct VisitorInterfaceString;
struct VisitorInterfaceInt;

struct Expr {
  virtual ~Expr(){}
  virtual std::string accept(VisitorInterfaceString& v) = 0;
  virtual int accept(VisitorInterfaceInt& v) = 0;
};

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

//so these interfaces need to be implemented when needed (int interface is just to test)
struct VisitorInterfaceString {
  virtual std::string visit(Binary& e) = 0;
  virtual std::string visit(Grouping& e) = 0;
  virtual std::string visit(Literal& e) = 0;
  virtual std::string visit(Unary& e) = 0;
};

struct VisitorInterfaceInt {
  virtual int visit(Binary& e) = 0;
  virtual int visit(Grouping& e) = 0;
  virtual int visit(Literal& e) = 0;
  virtual int visit(Unary& e) = 0;
};

struct Binary: public Expr {
  public:
    Binary(Expr* left, Token* oprtr, Expr* right): left(left), oprtr(oprtr), right(right) {}
    ~Binary() {
      if(!left) delete left;
      if(!oprtr) delete oprtr;
      if(!right) delete right;
    }
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
    int accept(VisitorInterfaceInt& v) { return v.visit(*this); }
  public:
    Expr* left;
    Token* oprtr;
    Expr* right;
};

struct Grouping: public Expr {
  public:
    Grouping(Expr* expr): expr(expr) {}
    ~Grouping() {
      if(!expr) delete expr;
    }
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
    int accept(VisitorInterfaceInt& v) { return v.visit(*this); }
  public:
    Expr* expr;
};

struct Literal: public Expr {
  public:
    Literal(std::string value): value(value) {}
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
    int accept(VisitorInterfaceInt& v) { return v.visit(*this); }
  public:
    std::string value;
};

struct Unary: public Expr {
  public:
    Unary(Token* oprtr, Expr* right): oprtr(oprtr), right(right) {}
    ~Unary() {
      if(!oprtr) delete oprtr;
      if(!right) delete right;
    }
    std::string accept(VisitorInterfaceString& v) { return v.visit(*this); }
    int accept(VisitorInterfaceInt& v) { return v.visit(*this); }
  public:
    Token* oprtr; //note: Token has a type, lexeme, literal (optional) and line 
    Expr* right;
};


}

#endif //EXPR_H
