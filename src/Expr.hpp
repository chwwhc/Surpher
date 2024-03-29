#ifndef SURPHER_EXPR_HPP
#define SURPHER_EXPR_HPP

#include <memory>
#include <any>
#include <vector>
#include <optional>

#include "Token.hpp"

struct Stmt;
struct Binary;
struct Ternary;
struct Group;
struct Literal;
struct Unary;
struct Assign;
struct Variable;
struct Logical;
struct Call;
struct Lambda;
struct Get;
struct Set;
struct This;
struct Super;
struct Array;
struct Access;
struct ArraySet;
struct Comma;
struct Pipe;

struct ExprVisitor
{
    virtual std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) = 0;

    virtual std::any visitGroupExpr(const std::shared_ptr<Group> &expr) = 0;

    virtual std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) = 0;

    virtual std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) = 0;

    virtual std::any visitAssignExpr(const std::shared_ptr<Assign> &expr) = 0;

    virtual std::any visitVariableExpr(const std::shared_ptr<Variable> &expr) = 0;

    virtual std::any visitLogicalExpr(const std::shared_ptr<Logical> &expr) = 0;

    virtual std::any visitCallExpr(const std::shared_ptr<Call> &expr) = 0;

    virtual std::any visitLambdaExpr(const std::shared_ptr<Lambda> &expr) = 0;

    virtual std::any visitTernaryExpr(const std::shared_ptr<Ternary> &expr) = 0;

    virtual std::any visitGetExpr(const std::shared_ptr<Get> &expr) = 0;

    virtual std::any visitSetExpr(const std::shared_ptr<Set> &expr) = 0;

    virtual std::any visitThisExpr(const std::shared_ptr<This> &expr) = 0;

    virtual std::any visitSuperExpr(const std::shared_ptr<Super> &expr) = 0;

    virtual std::any visitArrayExpr(const std::shared_ptr<Array> &expr) = 0;

    virtual std::any visitAccessExpr(const std::shared_ptr<Access> &expr) = 0;

    virtual std::any visitArraySetExpr(const std::shared_ptr<ArraySet> &expr) = 0;

    virtual std::any visitCommaExpr(const std::shared_ptr<Comma> &expr) = 0;

    virtual std::any visitPipeExpr(const std::shared_ptr<Pipe> &expr) = 0;
};

struct Expr
{
    virtual std::any accept(ExprVisitor &visitor) = 0;
};

struct Binary : Expr, public std::enable_shared_from_this<Binary>
{
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Pipe : Expr, public std::enable_shared_from_this<Pipe>
{
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    Pipe(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Logical : Expr, public std::enable_shared_from_this<Logical>
{
    const std::shared_ptr<Expr> left;
    const Token op;
    const std::shared_ptr<Expr> right;

    Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Group : Expr, public std::enable_shared_from_this<Group>
{
    const std::shared_ptr<Expr> expr_in;

    explicit Group(std::shared_ptr<Expr> expr);

    std::any accept(ExprVisitor &visitor) override;
};

struct Literal : Expr, public std::enable_shared_from_this<Literal>
{
    const std::any value;

    explicit Literal(std::any value);

    std::any accept(ExprVisitor &visitor) override;
};

struct Unary : Expr, public std::enable_shared_from_this<Unary>
{
    const Token op;
    const std::shared_ptr<Expr> right;

    Unary(Token op, std::shared_ptr<Expr> right);

    std::any accept(ExprVisitor &visitor) override;
};

struct Assign : Expr, public std::enable_shared_from_this<Assign>
{
    const Token name;
    const std::shared_ptr<Expr> value;

    Assign(Token name, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor &visitor) override;
};

struct Variable : Expr, public std::enable_shared_from_this<Variable>
{
    const Token name;
    const bool is_fixed;

    Variable(Token name, bool is_fixed);

    std::any accept(ExprVisitor &visitor) override;
};

struct Call : Expr, public std::enable_shared_from_this<Call>
{
    const std::shared_ptr<Expr> callee;
    const Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;

    Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments);

    std::any accept(ExprVisitor &visitor) override;
};

struct Lambda : Expr, public std::enable_shared_from_this<Lambda>
{
    const Token name;
    const std::vector<Token> params;
    const std::shared_ptr<Expr> body;

    Lambda(Token name, std::vector<Token> params, std::shared_ptr<Expr> body);

    std::any accept(ExprVisitor &visitor) override;
};

struct Ternary : Expr, public std::enable_shared_from_this<Ternary>
{
    const std::shared_ptr<Expr> condition;
    const Token question;
    const std::shared_ptr<Expr> true_branch;
    const Token colon;
    const std::shared_ptr<Expr> else_branch;

    Ternary(std::shared_ptr<Expr> condition, Token question, std::shared_ptr<Expr> true_branch, Token colon, std::shared_ptr<Expr> else_branch);

    std::any accept(ExprVisitor &visitor) override;
};

struct Get : Expr, public std::enable_shared_from_this<Get>
{
    const std::shared_ptr<Expr> object;
    const Token name;

    Get(std::shared_ptr<Expr> object, Token name);

    std::any accept(ExprVisitor &visitor) override;
};

struct Set : Expr, public std::enable_shared_from_this<Set>
{
    const std::shared_ptr<Expr> object;
    const Token name;
    const std::shared_ptr<Expr> value;

    Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value);

    std::any accept(ExprVisitor &visitor) override;
};

struct This : Expr, public std::enable_shared_from_this<This>
{
    const Token keyword;

    explicit This(Token keyword);

    std::any accept(ExprVisitor &visitor) override;
};

struct Super : Expr, public std::enable_shared_from_this<Super>
{
    const Token keyword;
    const Token method;

    Super(Token keyword, Token method);

    std::any accept(ExprVisitor &visitor) override;
};

struct Array : Expr, public std::enable_shared_from_this<Array>
{
    const Token op;
    const std::vector<std::shared_ptr<Expr>> expr_vector;
    uint64_t size;
    const std::shared_ptr<Expr> dynamic_size;

    Array(Token op, std::vector<std::shared_ptr<Expr>> expr_vector, std::shared_ptr<Expr> dynamic_size);

    void setArraySize(uint64_t new_size);

    std::any accept(ExprVisitor &visitor) override;
};

struct Access : Expr, public std::enable_shared_from_this<Access>
{
    const std::shared_ptr<Expr> index;
    const std::shared_ptr<Expr> arr_name;
    const Token op;

    Access(std::shared_ptr<Expr> index, std::shared_ptr<Expr> arr_name, Token op);

    std::any accept(ExprVisitor &visitor) override;
};

struct ArraySet : Expr, public std::enable_shared_from_this<ArraySet>
{
    const std::shared_ptr<Expr> assignee;
    const std::shared_ptr<Expr> value;
    const Token op;

    ArraySet(std::shared_ptr<Expr> assignee, std::shared_ptr<Expr> value, Token op);

    std::any accept(ExprVisitor &visitor) override;
};

struct Comma : Expr, public std::enable_shared_from_this<Comma>
{
    const std::vector<std::shared_ptr<Expr>> expressions;

    explicit Comma(std::vector<std::shared_ptr<Expr>> expressions);

    std::any accept(ExprVisitor &visitor) override;
};

#endif // SURPHER_EXPR_HPP
