#ifndef CPPSURPHER_EXPR_HPP
#define CPPSURPHER_EXPR_HPP

#include <memory>
#include <any>

#include "Token.hpp"

struct Binary;
struct Group;
struct Literal;
struct Unary;
struct Assign;
struct Variable;
struct Logical;

struct ExprVisitor {
    virtual std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) = 0;

    virtual std::any visitGroupExpr(const std::shared_ptr<Group> &expr) = 0;

    virtual std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) = 0;

    virtual std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) = 0;

    virtual std::any visitAssignExpr(const std::shared_ptr<Assign> &expr) = 0;

    virtual std::any visitVariableExpr(const std::shared_ptr<Variable> &expr) = 0;

    virtual std::any visitLogicalExpr(const std::shared_ptr<Logical> &expr) = 0;

    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual std::any accept(ExprVisitor &visitor) = 0;
};

struct Binary : Expr, public std::enable_shared_from_this<Binary> {
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Logical : Expr, public std::enable_shared_from_this<Logical>{
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Group : Expr, public std::enable_shared_from_this<Group> {
    const std::shared_ptr<Expr> expr_in;

    explicit Group(std::shared_ptr<Expr> expr);

    std::any accept(ExprVisitor &visitor) override;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal> {
    const std::any value;

    explicit Literal(std::any value);

    std::any accept(ExprVisitor &visitor) override;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary> {
    const Token op;
    const std::shared_ptr<Expr> right;

    Unary(Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Assign: Expr, public std::enable_shared_from_this<Assign>{
    const Token name;
    const std::shared_ptr<Expr> value;

    Assign(Token  name, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor &visitor) override;
};

struct Variable: Expr, public std::enable_shared_from_this<Variable> {
    const Token name;
    explicit Variable(Token  name);

    std::any accept(ExprVisitor& visitor) override;


};

#endif //CPPSURPHER_EXPR_HPP
