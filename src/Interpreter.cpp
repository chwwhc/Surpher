#include <cmath>

#include "Interpreter.hpp"

std::any Interpreter::visitLiteralExpr(const std::shared_ptr<Literal> &expr) {
    return expr->value;
}

std::any Interpreter::visitGroupExpr(const std::shared_ptr<Group> &expr) {
    return evaluate(expr);
}

std::any Interpreter::visitUnaryExpr(const std::shared_ptr<Unary> &expr) {
    std::any right = evaluate(expr->right);

    switch (expr->op.token_type) {
        case MINUS:
            check_number_operands(expr->op, {right});
            return -std::any_cast<double>(right);
        case BANG:
            return !is_truthy(right);
        default:
            return {};
    }
}

std::any Interpreter::visitBinaryExpr(const std::shared_ptr<Binary> &expr) {
    std::any left = evaluate(expr->left), right = evaluate(expr->right);
    switch (expr->op.token_type) {
        case MINUS:
            check_number_operands(expr->op, {left, right});
            return std::any_cast<double>(left) -
                   std::any_cast<double>(right);
        case SLASH:
            check_number_operands(expr->op, {left, right});
            check_zero(expr->op, {std::any_cast<double>(right)});
            return std::any_cast<double>(left) /
                   std::any_cast<double>(right);
        case STAR:
            check_number_operands(expr->op, {left, right});
            return std::any_cast<double>(left) *
                   std::any_cast<double>(right);
        case SINGLE_PLUS:
        {
            if(left.type() == typeid(double) && right.type() == typeid(double)){
                return std::any_cast<double>(left) +
                       std::any_cast<double>(right);
            }else if(left.type() == typeid(std::string) && right.type() == typeid(std::string)){
                return std::any_cast<std::string>(left) +
                       std::any_cast<std::string>(right);
            }else{
                throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
            }
        }
            check_number_operands(expr->op, {left, right});

        case CARET:
            check_number_operands(expr->op, {left, right});
            return std::pow(std::any_cast<double>(left), std::any_cast<double>(right));
        case PERCENT:
            check_number_operands(expr->op, {left, right});
            check_zero(expr->op, {std::any_cast<double>(right)});
            return std::fmod(std::any_cast<double>(left), std::any_cast<double>(right));
        case SINGLE_AMPERSAND:
            check_number_operands(expr->op, {left, right});
            return (long long) std::round(std::any_cast<double>(left)) &
                    (long long) std::round(std::any_cast<double>(right));
        case SINGLE_BAR:
            check_number_operands(expr->op, {left, right});
            return (long long) std::round(std::any_cast<double>(left)) |
                   (long long) std::round(std::any_cast<double>(right));
        case GREATER:
            check_number_operands(expr->op, {left, right});
            return std::any_cast<double>(left) >
                   std::any_cast<double>(right);
        case GREATER_EQUAL:
            check_number_operands(expr->op, {left, right});
            return std::any_cast<double>(left) >=
                   std::any_cast<double>(right);
        case LESS:
            check_number_operands(expr->op, {left, right});
            return std::any_cast<double>(left) <
                   std::any_cast<double>(right);
        case LESS_EQUAL:
            check_number_operands(expr->op, {left, right});
            return std::any_cast<double>(left) <=
                   std::any_cast<double>(right);
        case BANG_EQUAL:
            return !is_equal(left, right);
        case DOUBLE_EQUAL:
            return is_equal(left, right);
        default:
            throw std::invalid_argument("Unexpected value: "s + expr->op.lexeme);

    }
}

void Interpreter::interpret(const std::shared_ptr<Expr> &expression) {
    try {
        std::any val = evaluate(expression);
        std::cout << stringify(val) << std::endl;
    } catch (RuntimeError &e) {
        ::runtimeError(e);
    }
}
