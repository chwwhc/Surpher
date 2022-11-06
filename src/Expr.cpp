#include "Expr.hpp"

#include <utility>

Binary::Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(std::move(left)),
                                                                                    op(std::move(op)),
                                                                                    right(std::move(right))
{
}

std::any Binary::accept(ExprVisitor &visitor)
{
    return visitor.visitBinaryExpr(shared_from_this());
}

Group::Group(std::shared_ptr<Expr> expr_in) : expr_in(std::move(expr_in))
{
}

std::any Group::accept(ExprVisitor &visitor)
{
    return visitor.visitGroupExpr(shared_from_this());
}

Literal::Literal(std::any value) : value(std::move(value))
{
}

std::any Literal::accept(ExprVisitor &visitor)
{
    return visitor.visitLiteralExpr(shared_from_this());
}

Unary::Unary(Token op, std::shared_ptr<Expr> right) : op(std::move(op)), right(std::move(right))
{
}

std::any Unary::accept(ExprVisitor &visitor)
{
    return visitor.visitUnaryExpr(shared_from_this());
}

Assign::Assign(Token name, std::shared_ptr<Expr> value) : name{std::move(name)}, value{std::move(value)}
{
}

std::any Assign::accept(ExprVisitor &visitor)
{
    return visitor.visitAssignExpr(shared_from_this());
}

Variable::Variable(Token name, bool is_fixed) : name(std::move(name)), is_fixed(is_fixed)
{
}

std::any Variable::accept(ExprVisitor &visitor)
{
    return visitor.visitVariableExpr(shared_from_this());
}

Pipe::Pipe(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left{std::move(left)},
                                                                                op{std::move(op)},
                                                                                right{std::move(right)}
{
}

std::any Pipe::accept(ExprVisitor &visitor)
{
    return visitor.visitPipeExpr(shared_from_this());
}

Logical::Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left{std::move(left)},
                                                                                      op{std::move(op)},
                                                                                      right{std::move(right)}
{
}

std::any Logical::accept(ExprVisitor &visitor)
{
    return visitor.visitLogicalExpr(shared_from_this());
}

Call::Call(std::shared_ptr<Expr> callee, Token paren, std::vector<std::shared_ptr<Expr>> arguments) : callee(
                                                                                                          std::move(callee)),
                                                                                                      paren(std::move(paren)), arguments(std::move(arguments))
{
}

std::any Call::accept(ExprVisitor &visitor)
{
    return visitor.visitCallExpr(shared_from_this());
}

Lambda::Lambda(Token name, std::vector<Token> params, std::shared_ptr<Expr> body) : name(std::move(name)),
                                                                                    params(std::move(params)),
                                                                                    body(std::move(body))
{
}

std::any Lambda::accept(ExprVisitor &visitor)
{
    return visitor.visitLambdaExpr(shared_from_this());
}

Ternary::Ternary(std::shared_ptr<Expr> condition, Token question, std::shared_ptr<Expr> true_branch, Token colon,
                 std::shared_ptr<Expr> else_branch) : condition(std::move(condition)), question(std::move(question)),
                                                      true_branch(std::move(true_branch)), colon(std::move(colon)),
                                                      else_branch(std::move(else_branch))
{
}

std::any Ternary::accept(ExprVisitor &visitor)
{
    return visitor.visitTernaryExpr(shared_from_this());
}

Get::Get(std::shared_ptr<Expr> object, Token name) : object(std::move(object)), name(std::move(name))
{
}

std::any Get::accept(ExprVisitor &visitor)
{
    return visitor.visitGetExpr(shared_from_this());
}

Set::Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value) : object(std::move(object)),
                                                                                  name(std::move(name)),
                                                                                  value(std::move(value))
{
}

std::any Set::accept(ExprVisitor &visitor)
{
    return visitor.visitSetExpr(shared_from_this());
}

This::This(Token keyword) : keyword(std::move(keyword))
{
}

std::any This::accept(ExprVisitor &visitor)
{
    return visitor.visitThisExpr(shared_from_this());
}

Super::Super(Token keyword, Token method) : keyword(std::move(keyword)), method(std::move(method))
{
}

std::any Super::accept(ExprVisitor &visitor)
{
    return visitor.visitSuperExpr(shared_from_this());
}

Array::Array(Token op, std::vector<std::shared_ptr<Expr>> expr_vector, std::shared_ptr<Expr> dynamic_size) : op(std::move(op)), expr_vector(std::move(expr_vector)), dynamic_size(std::move(dynamic_size))
{
}

std::any Array::accept(ExprVisitor &visitor)
{
    return visitor.visitArrayExpr(shared_from_this());
}

void Array::setArraySize(uint64_t new_size)
{
    this->size = new_size;
}

Access::Access(std::shared_ptr<Expr> index, std::shared_ptr<Expr> arr_name, Token op) : index(std::move(index)), arr_name(std::move(arr_name)), op(std::move(op))
{
}

std::any Access::accept(ExprVisitor &visitor)
{
    return visitor.visitAccessExpr(shared_from_this());
}

ArraySet::ArraySet(std::shared_ptr<Expr> assignee, std::shared_ptr<Expr> value, Token op) : assignee(std::move(assignee)), value(std::move(value)), op(std::move(op))
{
}

std::any ArraySet::accept(ExprVisitor &visitor)
{
    return visitor.visitArraySetExpr(shared_from_this());
}

Comma::Comma(std::vector<std::shared_ptr<Expr>> expressions) : expressions(std::move(expressions)) {}

std::any Comma::accept(ExprVisitor &visitor)
{
    return visitor.visitCommaExpr(shared_from_this());
}