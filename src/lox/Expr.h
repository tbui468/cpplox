#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include "Token.h"
#include "Object.h"

namespace lox {

struct VisitorString;
struct VisitorObject;
struct ExprVisitorVoid;

struct Expr {
  virtual ~Expr(){}
  virtual std::string accept(VisitorString& v) = 0;
  virtual std::shared_ptr<Object> accept(VisitorObject& v) = 0;
  virtual void accept(ExprVisitorVoid& v) = 0;
};

struct Assign;
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;
struct Logical;
struct Call;
struct Get;
struct Set;

struct ExprVisitorVoid {
  virtual void visit(std::shared_ptr<Assign> e) = 0;
  virtual void visit(Binary& e) = 0;
  virtual void visit(Grouping& e) = 0;
  virtual void visit(std::shared_ptr<Literal> e) = 0;
  virtual void visit(Unary& e) = 0;
  virtual void visit(std::shared_ptr<Variable> e) = 0;
  virtual void visit(Logical& e) = 0;
  virtual void visit(Call& e) = 0;
  virtual void visit(std::shared_ptr<Get> e) = 0;
  virtual void visit(std::shared_ptr<Set> e) = 0;
};

struct VisitorString {
  virtual std::string visit(std::shared_ptr<Assign> e) = 0;
  virtual std::string visit(Binary& e) = 0;
  virtual std::string visit(Grouping& e) = 0;
  virtual std::string visit(std::shared_ptr<Literal> e) = 0;
  virtual std::string visit(Unary& e) = 0;
  virtual std::string visit(std::shared_ptr<Variable> e) = 0;
  virtual std::string visit(Logical& e) = 0;
  virtual std::string visit(Call& e) = 0;
  virtual std::string visit(std::shared_ptr<Get> e) = 0;
  virtual std::string visit(std::shared_ptr<Set> e) = 0;
};

struct VisitorObject {
  virtual std::shared_ptr<Object> visit(std::shared_ptr<Assign> e) = 0;
  virtual std::shared_ptr<Object> visit(Binary& e) = 0;
  virtual std::shared_ptr<Object> visit(Grouping& e) = 0;
  virtual std::shared_ptr<Object> visit(std::shared_ptr<Literal> e) = 0;
  virtual std::shared_ptr<Object> visit(Unary& e) = 0;
  virtual std::shared_ptr<Object> visit(std::shared_ptr<Variable> e) = 0;
  virtual std::shared_ptr<Object> visit(Logical& e) = 0;
  virtual std::shared_ptr<Object> visit(Call& e) = 0;
  virtual std::shared_ptr<Object> visit(std::shared_ptr<Get> e) = 0;
  virtual std::shared_ptr<Object> visit(std::shared_ptr<Set> e) = 0;
};


struct Assign: public Expr, public std::enable_shared_from_this<Assign> {
  public:
    Assign(Token name, std::shared_ptr<Expr> value): name(name), value(value) {}
    ~Assign() {}
    std::string accept(VisitorString& v) { return v.visit(shared_from_this()); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(shared_from_this()); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(shared_from_this()); }
  public:
    Token name;
    std::shared_ptr<Expr> value;
};

struct Binary: public Expr {
  public:
    Binary(Token oprtr, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right): oprtr(oprtr), left(left), right(right) {}
    ~Binary() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(*this); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(*this); }
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
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(*this); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(*this); }
  public:
    std::shared_ptr<Expr> expr;
};

struct Literal: public Expr, public std::enable_shared_from_this<Literal> {
  public:
    Literal(std::shared_ptr<Object> value): value(value) {}
    std::string accept(VisitorString& v) { return v.visit(shared_from_this()); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(shared_from_this()); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(shared_from_this()); }
  public:
    std::shared_ptr<Object> value;
};

struct Unary: public Expr {
  public:
    Unary(Token oprtr, std::shared_ptr<Expr> right): oprtr(oprtr), right(right) {}
    ~Unary() {}
    std::string accept(VisitorString& v) { return v.visit(*this); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(*this); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(*this); }
  public:
    Token oprtr;
    std::shared_ptr<Expr> right;
};

struct Variable: public Expr, public std::enable_shared_from_this<Variable> {
  public:
    Variable(Token name): name(name) {}
    ~Variable() {}
    std::string accept(VisitorString& v) { return v.visit(shared_from_this()); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(shared_from_this()); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(shared_from_this()); }
  public:
    Token name;
};

struct Logical: public Expr {
  public:
    Logical(Token oprtr, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right):
      oprtr(oprtr), left(left), right(right) {}
    ~Logical() {}
    std::string accept(VisitorString& visitor) { return visitor.visit(*this); }
    std::shared_ptr<Object> accept(VisitorObject& visitor) { return visitor.visit(*this); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(*this); }
  public:
    Token oprtr;
    std::shared_ptr<Expr> left;
    std::shared_ptr<Expr> right;
};

struct Call: public Expr {
  public:
    Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments):
      callee(callee), paren(paren), arguments(arguments) {}
    ~Call() {}
    std::string accept(VisitorString& visitor) { return visitor.visit(*this); }
    std::shared_ptr<Object> accept(VisitorObject& visitor) { return visitor.visit(*this); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(*this); }
  public:
    std::shared_ptr<Expr> callee; //any expression that evaluates to a function
    Token paren; //keeping closing token for runtime error reporting
    std::vector<std::shared_ptr<Expr>> arguments;
};

struct Get: public Expr, std::enable_shared_from_this<Get> {
  public:
    Get(std::shared_ptr<Expr> object, Token name): object(object), name(name) {}
    ~Get() {}
    std::string accept(VisitorString& v) { return v.visit(shared_from_this()); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(shared_from_this()); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(shared_from_this()); }
  public:
    std::shared_ptr<Expr> object;
    Token name;
};

struct Set: public Expr, std::enable_shared_from_this<Set> {
  public:
    Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value): 
      object(object), name(name), value(value) {}
    ~Set() {}
    std::string accept(VisitorString& v) { return v.visit(shared_from_this()); }
    std::shared_ptr<Object> accept(VisitorObject& v) { return v.visit(shared_from_this()); }
    void accept(ExprVisitorVoid& visitor) { return visitor.visit(shared_from_this()); }
  public:
    std::shared_ptr<Expr> object;
    Token name;
    std::shared_ptr<Expr> value;
};

}

#endif //EXPR_H
