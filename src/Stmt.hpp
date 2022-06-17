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
struct Break;
struct Continue;
struct Function;
struct Return;

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

struct Break : Stmt, public std::enable_shared_from_this<Break>{
    const Token break_tok;

    explicit Break(Token break_tok);
    std::any accept(StmtVisitor &visitor) override;
};

struct Continue : Stmt, public std::enable_shared_from_this<Continue>{
    const Token continue_tok;

    explicit Continue(Token continue_tok);
    std::any accept(StmtVisitor &visitor) override;
};

struct Function : Stmt, public std::enable_shared_from_this<Function>{
    const Token name;
    const std::vector<Token> params;
    const std::vector<std::shared_ptr<Stmt>> body;

    Function(Token name, std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body);

    std::any accept(StmtVisitor &visitor) override;
};

struct Return : Stmt, public std::enable_shared_from_this<Return>{
    const Token keyword;
    const std::shared_ptr<Expr> value;

    Return(Token keyword, std::shared_ptr<Expr> value);
    std::any accept(StmtVisitor &visitor) override;
};


#endif //SURPHER_STMT_HPP
