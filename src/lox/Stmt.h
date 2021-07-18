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
struct Var;
struct Block;
struct If;
struct While;
struct Function;
struct Return;

struct StmtVisitorString {
  virtual std::string visit(Expression& stmt) = 0;
  virtual std::string visit(Print& stmt) = 0;
  virtual std::string visit(Var& stmt) = 0;
  virtual std::string visit(Block& stmt) = 0;
  virtual std::string visit(If& stmt) = 0;
  virtual std::string visit(While& stmt) = 0;
  virtual std::string visit(Function& stmt) = 0;
  virtual std::string visit(Return& stmt) = 0;
};

struct StmtVisitorVoid {
  virtual void visit(Expression& stmt) = 0;
  virtual void visit(Print& stmt) = 0;
  virtual void visit(Var& stmt) = 0;
  virtual void visit(Block& stmt) = 0;
  virtual void visit(If& stmt) = 0;
  virtual void visit(While& stmt) = 0;
  virtual void visit(Function& stmt) = 0;
  virtual void visit(Return& stmt) = 0;
};

struct Expression: public Stmt {
  public:
    Expression(std::shared_ptr<Expr> expr): expr(expr) {}
    ~Expression() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    std::shared_ptr<Expr> expr;
};

struct Print: public Stmt {
  public:
    Print(std::shared_ptr<Expr> expr): expr(expr) {}
    ~Print() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    std::shared_ptr<Expr> expr;
};

struct Var: public Stmt {
  public:
    Var(Token token, std::shared_ptr<Expr> expr): name(token), initializer(expr) {}
    ~Var() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    Token name;
    std::shared_ptr<Expr> initializer;
};


struct Block: public Stmt {
  public:
    Block(std::vector<std::shared_ptr<Stmt>> statements): statements(statements) {}
    ~Block() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    std::vector<std::shared_ptr<Stmt>> statements;
};

struct If: public Stmt {
   public:
     If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch):
       condition(condition), then_branch(then_branch), else_branch(else_branch) {}
     ~If() {}
     std::string accept(StmtVisitorString& visitor) { return visitor.visit(*this); }
     void accept(StmtVisitorVoid& visitor) { return visitor.visit(*this); }
   public:
     std::shared_ptr<Expr> condition;
     std::shared_ptr<Stmt> then_branch;
     std::shared_ptr<Stmt> else_branch;
};

struct While: public Stmt {
  public:
    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body):
      condition(condition), body(body) {}
    ~While() {}
    std::string accept(StmtVisitorString& visitor) { return visitor.visit(*this); }
    void accept(StmtVisitorVoid& visitor) { visitor.visit(*this); }
  public:
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;
};

struct Function: public Stmt {
  public:
    Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body): 
      name(name), params(params), body(body) {}
    ~Function() {}
    std::string accept(StmtVisitorString& v) { return v.visit(*this); }
    void accept(StmtVisitorVoid& v) { return v.visit(*this); }
  public:
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;
};

struct Return: public Stmt {
  public:
    Return(Token keyword, std::shared_ptr<Expr> value): keyword(keyword), value(value) {}
    ~Return() {}
    std::string accept(StmtVisitorString& visitor) { return visitor.visit(*this); }
    void accept(StmtVisitorVoid& visitor) { return visitor.visit(*this); }
  public:
    Token keyword;
    std::shared_ptr<Expr> value;
};


}

#endif //STMT_H
