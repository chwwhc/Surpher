
#ifndef CPPSURPHER_ASTPRINTER_HPP
#define CPPSURPHER_ASTPRINTER_HPP

#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include <typeinfo>

#include "Expr.hpp"

class AstPrinter : public ExprVisitor {
    std::function<std::string(const std::shared_ptr<Expr>)> print = [&](auto expr) {
        return std::any_cast<std::string>(expr->accept(*this));
    };

    template<class... E>
    std::string parenthesize(std::string_view name, E... expr);

public:
    std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) override;

    std::any visitGroupExpr(const std::shared_ptr<Group> &expr) override;

    std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) override;

    std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) override;

    std::string printAst(const std::shared_ptr<Expr> &expr);
};



#endif //CPPSURPHER_ASTPRINTER_HPP
