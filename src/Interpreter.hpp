#ifndef CPPSURPHER_INTERPRETER_HPP
#define CPPSURPHER_INTERPRETER_HPP

#include <functional>
#include <numeric>
#include "Expr.hpp"
#include "Error.hpp"

using namespace std::string_literals;

class Interpreter : public ExprVisitor {
    std::function<std::any(const std::shared_ptr<Expr>)> evaluate = [&, this](auto expr) {
        return expr->accept(*this);
    };
    std::function<bool(const std::any)> is_truthy = [&](auto val) {
        if (val.type() == typeid(nullptr)) {
            return false;
        }
        if (val.type() == typeid(bool)) {
            return std::any_cast<bool>(val);
        }
        return true;
    };
    std::function<bool(const std::any, const std::any)> is_equal = [&](auto a, auto b) {
        if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr)) {
            return true;
        }
        if (a.type() == typeid(nullptr)) {
            return false;
        }
        if (a.type() == typeid(std::string) && b.type() == typeid(std::string)) {
            return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
        }
        if (a.type() == typeid(double) && b.type() == typeid(double)) {
            return std::any_cast<double>(a) == std::any_cast<double>(b);
        }
        if (a.type() == typeid(bool) && b.type() == typeid(bool)) {
            return std::any_cast<bool>(a) == std::any_cast<bool>(b);
        }
        return false;
    };
    std::function<std::string(const std::any)> stringify = [&](auto val) {
        if (val.type() == typeid(nullptr)) {
            return "none"s;
        }
        if (val.type() == typeid(double)) {
            std::string number_str = std::to_string(std::any_cast<double>(val));
            if (number_str[number_str.length() - 2] == '.' && number_str[number_str.length() - 1] == '0') {
                number_str = number_str.substr(0, number_str.length() - 2);
            }
            return number_str;
        }
        if (val.type() == typeid(std::string)) {
            return std::any_cast<std::string>(val);
        }
        if (val.type() == typeid(bool)) {
            return std::any_cast<bool>(val) ? "true"s : "false"s;
        }
        if (val.type() == typeid(long long)) {
            return std::to_string(std::any_cast<long long>(val));
        }
        return "Error in stringify: un-recognized literal type."s;
    };
    std::function<void(const Token, const std::vector<std::any> &operands)> check_number_operands = [&](
            auto operator_token, auto operands) {
        bool result = std::transform_reduce(
                operands.begin(), operands.end(), true,
                [&](const bool &a, const bool &b) {
                    return a && b;
                },
                [&](const std::any &operand) { return operand.type() == typeid(double); }
        );
        if (result) {
            return;
        }
        throw RuntimeError{operator_token, "Operand must be a number."};
    };
    std::function<void(const Token, const std::vector<double>)> check_zero = [&](auto operator_token, auto operands) {
        bool result = std::transform_reduce(
                operands.begin(), operands.end(), true,
                [&](const bool &a, const bool &b) {
                    return a && b;
                },
                [&](const double &operand) { return operand != 0; }
        );
        if (result) {
            return;
        }
        throw RuntimeError(operator_token, "Denominator must be non-zero.");
    };

public:
    std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) override;

    std::any visitGroupExpr(const std::shared_ptr<Group> &expr) override;

    std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) override;

    std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) override;

    void interpret(const std::shared_ptr<Expr> &expression);
};

#endif //CPPSURPHER_INTERPRETER_HPP
