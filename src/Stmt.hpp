#ifndef SURPHER_STMT_HPP
#define SURPHER_STMT_HPP


#include <vector>
#include "Expr.hpp"

struct Block;
struct Expression;
struct Print;
struct Var;
struct If;
struct While;

struct StmtVisitor {
    virtual std::any visitBlockStmt(const std::shared_ptr<Block> &stmt) = 0;

    virtual std::any visitExpressionStmt(const std::shared_ptr<Expression> &stmt) = 0;

    virtual std::any visitPrintStmt(const std::shared_ptr<Print> &stmt) = 0;

    virtual std::any visitVarStmt(const std::shared_ptr<Var> &stmt) = 0;

    virtual std::any visitIfStmt(const std::shared_ptr<If> &stmt) = 0;

    virtual std::any visitWhileStmt(const std::shared_ptr<While> &stmt) = 0;

    virtual ~StmtVisitor() = default;
};

struct Stmt {
    virtual std::any accept(StmtVisitor &visitor) = 0;
};

struct Block : Stmt, public std::enable_shared_from_this<Block> {
    const std::vector<std::shared_ptr<Stmt>> statements;

    explicit Block(std::vector<std::shared_ptr<Stmt>> statements);


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
    const std::shared_ptr<Stmt> then_branch;
    const std::shared_ptr<Stmt> else_branch;

    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch);

    std::any accept(StmtVisitor &visitor) override;
};

struct While : Stmt, public std::enable_shared_from_this<While> {
    const std::shared_ptr<Expr> condition;
    const std::shared_ptr<Stmt> body;

    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);

    std::any accept(StmtVisitor &visitor) override;
};


#endif //SURPHER_STMT_HPP
