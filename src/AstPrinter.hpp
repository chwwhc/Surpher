
#ifndef CPPSURPHER_ASTPRINTER_HPP
#define CPPSURPHER_ASTPRINTER_HPP

#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include <typeinfo>

#include "Expr.hpp"

class AstPrinter : public ExprVisitor {
    template<class... E>
    std::string parenthesize(std::string_view name, E... expr);

    std::string print(const std::shared_ptr<Expr> &expr);

public:
    std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) override;

    std::any visitGroupExpr(const std::shared_ptr<Group> &expr) override;

    std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) override;

    std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) override;

    std::string printAst(const std::shared_ptr<Expr> &expr);
};



#endif //CPPSURPHER_ASTPRINTER_HPP
