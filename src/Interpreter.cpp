#include <cmath>

#include "Interpreter.hpp"

std::any Interpreter::visitLiteralExpr(const std::shared_ptr<Literal> &expr) {
    return expr->value;
}

std::any Interpreter::visitGroupExpr(const std::shared_ptr<Group> &expr) {
    return evaluate(expr->expr_in);
}

std::any Interpreter::visitUnaryExpr(const std::shared_ptr<Unary> &expr) {
    std::any right = evaluate(expr->right);

    switch (expr->op.token_type) {
        case MINUS:
            checkNumberOperands(expr->op, {right});
            return -std::any_cast<double>(right);
        case BANG:
            return !isTruthy(right);
        default:
            return {};
    }
}

std::any Interpreter::visitBinaryExpr(const std::shared_ptr<Binary> &expr) {
    std::any left = evaluate(expr->left), right = evaluate(expr->right);
    switch (expr->op.token_type) {
        case MINUS:
            checkNumberOperands(expr->op, {left, right});
            return std::any_cast<double>(left) -
                   std::any_cast<double>(right);
        case SLASH:
            checkNumberOperands(expr->op, {left, right});
            checkZero(expr->op, {std::any_cast<double>(right)});
            return std::any_cast<double>(left) /
                   std::any_cast<double>(right);
        case STAR:
            checkNumberOperands(expr->op, {left, right});
            return std::any_cast<double>(left) *
                   std::any_cast<double>(right);
        case SINGLE_PLUS: {
            if (left.type() == typeid(double) && right.type() == typeid(double)) {
                return std::any_cast<double>(left) +
                       std::any_cast<double>(right);
            } else if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
                return std::any_cast<std::string>(left) +
                       std::any_cast<std::string>(right);
            } else {
                throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
            }
        }
        case CARET:
            checkNumberOperands(expr->op, {left, right});
            return (long long) std::any_cast<double>(left) ^ (long long) std::any_cast<double>(right);
        case PERCENT:
            checkNumberOperands(expr->op, {left, right});
            checkZero(expr->op, {std::any_cast<double>(right)});
            return std::fmod(std::any_cast<double>(left), std::any_cast<double>(right));
        case SINGLE_AMPERSAND:
            checkNumberOperands(expr->op, {left, right});
            return (long long) std::round(std::any_cast<double>(left)) &
                   (long long) std::round(std::any_cast<double>(right));
        case SINGLE_BAR:
            checkNumberOperands(expr->op, {left, right});
            return (long long) std::round(std::any_cast<double>(left)) |
                   (long long) std::round(std::any_cast<double>(right));
        case GREATER:
            checkNumberOperands(expr->op, {left, right});
            return std::any_cast<double>(left) >
                   std::any_cast<double>(right);
        case GREATER_EQUAL:
            checkNumberOperands(expr->op, {left, right});
            return std::any_cast<double>(left) >=
                   std::any_cast<double>(right);
        case LESS:
            checkNumberOperands(expr->op, {left, right});
            return std::any_cast<double>(left) <
                   std::any_cast<double>(right);
        case LESS_EQUAL:
            checkNumberOperands(expr->op, {left, right});
            return std::any_cast<double>(left) <=
                   std::any_cast<double>(right);
        case BANG_EQUAL:
            return !isEqual(left, right);
        case DOUBLE_EQUAL:
            return isEqual(left, right);
        default:
            throw std::invalid_argument("Unexpected value: "s + expr->op.lexeme);

    }
}

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>> &statements) {
    try {
        for (const std::shared_ptr<Stmt> &statement: statements) {
            execute(statement);
        }
    } catch (RuntimeError &e) {
        runtimeError(e);
    } catch(BreakError &e){
        breakError(e);
    }catch (ContinueError &e){
        continueError(e);
    }
}

std::any Interpreter::visitExpressionStmt(const std::shared_ptr<Expression> &stmt) {
    evaluate(stmt->expression);
    return {};
}

std::any Interpreter::visitPrintStmt(const std::shared_ptr<Print> &stmt) {
    std::any val = evaluate(stmt->expression);
    std::cout << stringify(val) << std::endl;
    return {};
}

std::any Interpreter::visitBlockStmt(const std::shared_ptr<Block> &stmt) {
    executeBlock(stmt->statements, std::make_shared<Environment>(this->environment));
    return {};
}

std::any Interpreter::visitVarStmt(const std::shared_ptr<Var> &stmt) {
    std::any val;
    if (stmt->initializer != nullptr) {
        val = evaluate(stmt->initializer);
    }
    environment->define(stmt->name.lexeme, val);
    return {};
}

std::any Interpreter::visitVariableExpr(const std::shared_ptr<Variable> &expr) {
    return environment->get(expr->name);
}

std::any Interpreter::visitAssignExpr(const std::shared_ptr<Assign> &expr) {
    std::any val = evaluate(expr->value);
    environment->assign(expr->name, val);
    return val;
}

std::any Interpreter::evaluate(const std::shared_ptr<Expr> &expr) {
    return expr->accept(*this);
}

void Interpreter::execute(const std::shared_ptr<Stmt> &stmt) {
    stmt->accept(*this);
}

void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt>> &stmts,
                               const std::shared_ptr<Environment> &curr_environment) {
    auto previous = this->environment;
    try {
        this->environment = curr_environment;
        for (const std::shared_ptr<Stmt> &s: stmts) {
            execute(s);
        }
    } catch (...) {
        this->environment = previous;
        throw;
    }
    this->environment = previous;
}

bool Interpreter::isTruthy(const std::any &val) {
    if (val.type() == typeid(nullptr)) {
        return false;
    }
    if (val.type() == typeid(bool)) {
        return std::any_cast<bool>(val);
    }
    return true;
}

bool Interpreter::isEqual(const std::any &a, const std::any &b) {
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
}

std::string Interpreter::stringify(const std::any &val) {
    if (val.type() == typeid(void)) {
        return "none"s;
    }
    if (val.type() == typeid(double)) {
        auto double_val = std::any_cast<double>(val);
        std::string num_str = std::to_string(double_val);
        if(floor(double_val) == double_val){
            uint32_t point_index = 0;
            while(point_index < num_str.size() && num_str[point_index] != '.'){
                point_index++;
            }
            return num_str.substr(0, point_index);
        }
        return num_str;
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
}

void Interpreter::checkNumberOperands(const Token &operator_token, const std::vector<std::any> &operands) {
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
}

void Interpreter::checkZero(const Token &operator_token, const std::vector<double> &operands) {
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
}

std::any Interpreter::visitIfStmt(const std::shared_ptr<If> &stmt) {
    if(isTruthy(evaluate(stmt->condition))){
        execute(stmt->then_branch);
    }else if(stmt->else_branch != nullptr){
        execute(stmt->else_branch);
    }
    return {};
}

std::any Interpreter::visitLogicalExpr(const std::shared_ptr<Logical> &expr) {
    std::any left = evaluate(expr->left);

    if(expr->op.token_type == DOUBLE_BAR){
        if(isTruthy(left)){
            return left;
        }
    }else{
        if(!isTruthy(left)){
            return left;
        }
    }

    return evaluate(expr->right);
}

std::any Interpreter::visitWhileStmt(const std::shared_ptr<While> &stmt) {
    while(isTruthy(evaluate(stmt->condition))){
        try{
            execute(stmt->body);
        } catch (BreakError &e) {
            break;
        } catch(ContinueError &e){
            continue;
        }
    }
    return {};
}

std::any Interpreter::visitBreakStmt(const std::shared_ptr<Break> &stmt) {
    throw BreakError{stmt->break_tok, "'break' must be used in loop"};
}

std::any Interpreter::visitContinueStmt(const std::shared_ptr<Continue> &stmt) {
    throw ContinueError{stmt->continue_tok, "'continue' must be used in loop"};
}

std::any Interpreter::visitCallExpr(const std::shared_ptr<Call> &expr) {
    std::any callee = evaluate(expr->callee);

    std::vector<std::any> arguments;
    for(const auto& argument: expr->arguments){
        arguments.emplace_back(evaluate(argument));
    }

    std::shared_ptr<SurpherCallable> function;

    if(callee.type() == typeid(std::shared_ptr<SurpherFunction>)){
        function = std::any_cast<std::shared_ptr<SurpherFunction>>(callee);
    }else{
        throw RuntimeError(expr->paren, "Can only call functions and classes.");
    }

    if(arguments.size() != function->arity()){
        throw RuntimeError(expr->paren, "Expected " + std::to_string(function->arity()) + " arguments but got " + std::to_string(arguments.size()) + ".");
    }

    return function->call(*this, arguments);
}

Interpreter::Interpreter() : globals(new Environment){
    globals->define("clock", std::shared_ptr<Clock>{});
}

std::any Interpreter::visitFunctionStmt(const std::shared_ptr<Function> &stmt) {
    auto function = std::make_shared<SurpherFunction>(stmt, environment);
    environment->define(stmt->name.lexeme, function);
    return {};
}

std::any Interpreter::visitReturnStmt(const std::shared_ptr<Return> &stmt) {
    std::any value{};
    if(stmt->value != nullptr){
        value = evaluate(stmt->value);
    }

    throw ReturnError(value);
}

std::any Interpreter::visitLambdaExpr(const std::shared_ptr<Lambda> &expr) {
    auto function = std::make_shared<SurpherFunction>(std::make_shared<Function>(expr->name, expr->params, expr->body), environment);
    environment->define(expr->name.lexeme, function);
    return function;
}

