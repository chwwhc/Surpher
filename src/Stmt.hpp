#ifndef SURPHER_STMT_HPP
#define SURPHER_STMT_HPP


#include <vector>
#include <optional>
#include <list>
#include "Expr.hpp"

struct Block;
struct Expression;
struct Print;
struct Var;
struct If;
struct While;
struct Break;
struct Continue;
struct Function;
struct Return;
struct Class;
struct Import;

struct StmtVisitor {
    virtual std::any visitBlockStmt(const std::shared_ptr<Block> &stmt) = 0;

    virtual std::any visitExpressionStmt(const std::shared_ptr<Expression> &stmt) = 0;

    virtual std::any visitPrintStmt(const std::shared_ptr<Print> &stmt) = 0;

    virtual std::any visitVarStmt(const std::shared_ptr<Var> &stmt) = 0;

    virtual std::any visitIfStmt(const std::shared_ptr<If> &stmt) = 0;

    virtual std::any visitWhileStmt(const std::shared_ptr<While> &stmt) = 0;

    virtual std::any visitBreakStmt(const std::shared_ptr<Break> &stmt) = 0;

    virtual std::any visitContinueStmt(const std::shared_ptr<Continue> &stmt) = 0;

    virtual std::any visitFunctionStmt(const std::shared_ptr<Function> &stmt) = 0;

    virtual std::any visitReturnStmt(const std::shared_ptr<Return> &stmt) = 0;

    virtual std::any visitClassStmt(const std::shared_ptr<Class> &stmt) = 0;

    virtual std::any visitImportStmt(const std::shared_ptr<Import> &stmt) = 0;
};

struct Stmt {
    virtual std::any accept(StmtVisitor &visitor) = 0;
};

struct Block : Stmt, public std::enable_shared_from_this<Block> {
    const std::list<std::shared_ptr<Stmt>> statements;

    explicit Block(std::list<std::shared_ptr<Stmt>> statements);


    std::any accept(StmtVisitor &visitor) override;


};

struct Expression : Stmt, public std::enable_shared_from_this<Expression> {
    const std::shared_ptr<Expr> expression;

    explicit Expression(std::shared_ptr<Expr> expression);


    std::any accept(StmtVisitor &visitor) override;


};

struct Print : Stmt, public std::enable_shared_from_this<Print> {
    const std::shared_ptr<Expr> expression;

    explicit Print(std::shared_ptr<Expr> expression);


    std::any accept(StmtVisitor &visitor) override;


};

struct Var : Stmt, public std::enable_shared_from_this<Var> {
    const Token name;
    const std::shared_ptr<Expr> initializer;

    Var(Token name, std::shared_ptr<Expr> initializer);


    std::any accept(StmtVisitor &visitor) override;
};

struct If : Stmt, public std::enable_shared_from_this<If> {
    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> true_branch;
    const std::optional<std::shared_ptr<Stmt>> else_branch;

    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> true_branch,
       std::optional<std::shared_ptr<Stmt>> else_branch);

    std::any accept(StmtVisitor &visitor) override;
};

struct While : Stmt, public std::enable_shared_from_this<While> {
    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> body;

    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);

    std::any accept(StmtVisitor &visitor) override;
};

struct Break : Stmt, public std::enable_shared_from_this<Break> {
    const Token break_tok;

    explicit Break(Token break_tok);

    std::any accept(StmtVisitor &visitor) override;
};

struct Continue : Stmt, public std::enable_shared_from_this<Continue> {
    const Token continue_tok;

    explicit Continue(Token continue_tok);

    std::any accept(StmtVisitor &visitor) override;
};

struct Function : Stmt, public std::enable_shared_from_this<Function> {
    const Token name;
    const std::vector<Token> params;
    const std::list<std::shared_ptr<Stmt>> body;
    const bool is_virtual;

    Function(Token name, std::vector<Token> params, std::list<std::shared_ptr<Stmt>> body, bool is_virtual);

    std::any accept(StmtVisitor &visitor) override;
};

struct Return : Stmt, public std::enable_shared_from_this<Return> {
    const Token keyword;
    const std::optional<std::shared_ptr<Expr>> value;

    Return(Token keyword, std::optional<std::shared_ptr<Expr>> value);

    std::any accept(StmtVisitor &visitor) override;
};

struct Import : Stmt, public std::enable_shared_from_this<Import> {
    const Token keyword;
    const std::string script;

    Import(Token keyword, std::string script);

    std::any accept(StmtVisitor &visitor) override;
};

struct Class : Stmt, public std::enable_shared_from_this<Class> {
    const Token name;
    const std::optional<std::shared_ptr<Variable>> superclass;
    const std::vector<std::shared_ptr<Function>> instance_methods;
    const std::vector<std::shared_ptr<Function>> class_methods;

    Class(Token name, std::vector<std::shared_ptr<Function>> instance_methods,
          std::vector<std::shared_ptr<Function>> class_methods, std::optional<std::shared_ptr<Variable>> superclass);

    std::any accept(StmtVisitor &visitor) override;
};


#endif //SURPHER_STMT_HPP
