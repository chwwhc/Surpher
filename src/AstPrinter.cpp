#include "AstPrinter.hpp"

using namespace std::string_literals;

template<class... E>
std::string AstPrinter::parenthesize(std::string_view name, E... expr) {
    std::ostringstream str;

    str << "(" << name;
    ((str << " " << print(expr)), ...);
    str << ")";

    return str.str();
}

std::any AstPrinter::visitBinaryExpr(const std::shared_ptr<Binary> &expr) {
    return parenthesize(expr->op.lexeme, expr->left, expr->right);
}

std::any AstPrinter::visitGroupExpr(const std::shared_ptr<Group> &expr) {
    return parenthesize("Group", expr->expr_in);
}

std::any AstPrinter::visitLiteralExpr(const std::shared_ptr<Literal> &expr) {
    if (expr->value.type() == typeid(double)) {
        return std::to_string(std::any_cast<double>(expr->value));
    } else if (expr->value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(expr->value);
    } else if (expr->value.type() == typeid(bool)) {
        return std::any_cast<bool>(expr->value) ? "true"s : "false"s;
    } else if (expr->value.type() == typeid(nullptr)) {
        return "none"s;
    }
    return "Error occurred when trying to print the AST: unrecognized Surpher Literal type.";
}

std::any AstPrinter::visitUnaryExpr(const std::shared_ptr<Unary> &expr) {
    return parenthesize(expr->op.lexeme, expr->right);
}

std::string AstPrinter::printAst(const std::shared_ptr<Expr> &expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}

std::string AstPrinter::print(const std::shared_ptr<Expr> &expr) {
    return std::any_cast<std::string>(expr->accept(*this));
}
/*
    int main(int argc, char *argv[]) {
        std::shared_ptr<Expr> expression = std::make_shared<Binary>(
                std::make_shared<Unary>(
                        Token{"-", std::monostate{}, MINUS, 1},
                        std::make_shared<Literal>(123.)
                ),
                Token{"*", std::monostate{}, STAR, 1},
                std::make_shared<Group>(
                        std::make_shared<Literal>(45.67)));

        std::cout << AstPrinter{}.printAst(expression) << "\n";
    }
*/