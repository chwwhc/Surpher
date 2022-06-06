#include "Expr.hpp"

#include <utility>

Binary::Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right) : left(std::move(left)), op(std::move(op)), right(std::move(right)){

}

std::any Binary::accept(ExprVisitor &visitor) {
    return visitor.visitBinaryExpr(shared_from_this());
}

Group::Group(std::shared_ptr<Expr> expr_in) : expr_in(std::move(expr_in)){
}

std::any Group::accept(ExprVisitor &visitor) {
    return visitor.visitGroupExpr(shared_from_this());
}

Literal::Literal(std::any value) : value(std::move(value)){
}

std::any Literal::accept(ExprVisitor &visitor) {
    return visitor.visitLiteralExpr(shared_from_this());
}


Unary::Unary(Token op, std::shared_ptr<Expr> right) : op(std::move(op)), right(std::move(right)){
}

std::any Unary::accept(ExprVisitor &visitor) {
    return visitor.visitUnaryExpr(shared_from_this());
}


