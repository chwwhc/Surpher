#include <cmath>
#include <memory>

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
            } else if (left.type() == typeid(std::string) || right.type() == typeid(std::string)) {
                return stringify(left) +
                       stringify(right);
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
    } catch (BreakError &e) {
        breakError(e);
    } catch (ContinueError &e) {
        continueError(e);
    }
}

std::any Interpreter::visitExpressionStmt(const std::shared_ptr<Expression> &stmt) {
    evaluate(stmt->expression);
    return {};
}

std::any Interpreter::visitPrintStmt(const std::shared_ptr<Print> &stmt) {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << std::endl;
    return {};
}

std::any Interpreter::visitBlockStmt(const std::shared_ptr<Block> &stmt) {
    executeBlock(stmt->statements, std::make_shared<Environment>(this->environment));
    return {};
}

std::any Interpreter::visitVarStmt(const std::shared_ptr<Var> &stmt) {
    std::any value;
    if (stmt->initializer != nullptr) {
        value = evaluate(stmt->initializer);
    }
    environment->define(stmt->name.lexeme, std::move(value));
    return {};
}

std::any Interpreter::visitVariableExpr(const std::shared_ptr<Variable> &expr) {
    return lookUpVariable(expr->name, expr);
}

std::any Interpreter::visitAssignExpr(const std::shared_ptr<Assign> &expr) {
    std::any value = evaluate(expr->value);

    auto elem_iter = locals.find(expr);
    if (elem_iter != locals.end()) {
        environment->assignAt(elem_iter->second, expr->name, value);
    } else {
        globals->assign(expr->name, value);
    }

    return value;
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

bool Interpreter::isTruthy(const std::any &value) {
    if (value.type() == typeid(nullptr)) {
        return false;
    }
    if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value);
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

std::string Interpreter::stringify(const std::any &value) {
    if (value.type() == typeid(void)) {
        return "nil"s;
    } else if (value.type() == typeid(double)) {
        auto double_val = std::any_cast<double>(value);
        std::string num_str = std::to_string(double_val);
        if (floor(double_val) == double_val) {
            uint32_t point_index = 0;
            while (point_index < num_str.size() && num_str[point_index] != '.') {
                point_index++;
            }
            return num_str.substr(0, point_index);
        }
        return num_str;
    } else if (value.type() == typeid(std::string)) {
        return std::any_cast<std::string>(value);
    } else if (value.type() == typeid(bool)) {
        return std::any_cast<bool>(value) ? "true"s : "false"s;
    } else if (value.type() == typeid(long long)) {
        return std::to_string(std::any_cast<long long>(value));
    } else if (value.type() == typeid(std::shared_ptr<SurpherCallable>)) {
        return (std::any_cast<std::shared_ptr<SurpherCallable>>(value))->SurpherCallableToString();
    } else if (value.type() == typeid(std::shared_ptr<SurpherInstance>)) {
        return (std::any_cast<std::shared_ptr<SurpherInstance>>(value))->SurpherInstanceToString();
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
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->true_branch);
    } else if (stmt->else_branch.has_value()) {
        execute(stmt->else_branch.value());
    }
    return {};
}

std::any Interpreter::visitLogicalExpr(const std::shared_ptr<Logical> &expr) {
    std::any left = evaluate(expr->left);

    if (expr->op.token_type == OR) {
        if (isTruthy(left)) {
            return left;
        }
    } else {
        if (!isTruthy(left)) {
            return left;
        }
    }

    return evaluate(expr->right);
}

std::any Interpreter::visitWhileStmt(const std::shared_ptr<While> &stmt) {
    while (isTruthy(evaluate(stmt->condition))) {
        try {
            execute(stmt->body);
        } catch (BreakError &e) {
            break;
        } catch (ContinueError &e) {
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
    for (const auto &argument: expr->arguments) {
        arguments.emplace_back(evaluate(argument));
    }

    std::shared_ptr<SurpherCallable> callable;

    if (callee.type() == typeid(std::shared_ptr<SurpherFunction>)) {
        callable = std::any_cast<std::shared_ptr<SurpherFunction>>(callee);

        if (arguments.size() > callable->arity()) {
            throw RuntimeError(expr->paren, "Expected " + std::to_string(callable->arity()) + " arguments but got " +
                                            std::to_string(arguments.size()) + ".");
        }else if (arguments.size() < callable->arity()) {
            std::shared_ptr<SurpherFunction> old_fun = std::static_pointer_cast<SurpherFunction>(callable);
            std::shared_ptr<Function> partial_fun = std::make_shared<Function>(Token("0" + old_fun->declaration->name.lexeme, old_fun->declaration->name.literal, old_fun->declaration->name.token_type, old_fun->declaration->name.line),
                                                                               std::vector<Token>{old_fun->declaration->params.begin() + arguments.size(), old_fun->declaration->params.end()},
                                                          old_fun->declaration->body);
            for (size_t i = 0; i < arguments.size(); i++) {
                old_fun->closure->define(old_fun->declaration->params[i].lexeme, arguments[i]);
            }
            auto new_fun = std::make_shared<SurpherFunction>(partial_fun, old_fun->closure, old_fun->is_initializer, true);
            return new_fun;
        }else{
            return callable->call(*this, arguments);
        }
    } else if (callee.type() == typeid(std::shared_ptr<SurpherClass>)) {
        callable = std::any_cast<std::shared_ptr<SurpherClass>>(callee);
    } else if (callee.type() == typeid(std::shared_ptr<Clock>)) {
        callable = std::make_shared<Clock>();
    } else {
        throw RuntimeError(expr->paren, "Not a callable instance.");
    }

    if (arguments.size() != callable->arity()) {
        throw RuntimeError(expr->paren, "Expected " + std::to_string(callable->arity()) + " arguments but got " +
                                        std::to_string(arguments.size()) + ".");
    }

    return callable->call(*this, arguments);
}

Interpreter::Interpreter() : globals(new Environment) {
    globals->define("clock", std::shared_ptr<Clock>{});
}

std::any Interpreter::visitFunctionStmt(const std::shared_ptr<Function> &stmt) {
    auto function = std::make_shared<SurpherFunction>(stmt, environment, false, false);
    environment->define(stmt->name.lexeme, std::move(function));
    return {};
}

std::any Interpreter::visitReturnStmt(const std::shared_ptr<Return> &stmt) {
    std::any value{};
    if (stmt->value.has_value()) {
        value = evaluate(stmt->value.value());
    }

    throw ReturnError(value);
}

std::any Interpreter::visitLambdaExpr(const std::shared_ptr<Lambda> &expr) {
    std::vector<std::shared_ptr<Stmt>> lambda_return{std::make_shared<Return>(Token("return", {}, RETURN, expr->name.line), expr->body)};
    auto function = std::make_shared<SurpherFunction>(
            std::make_shared<Function>(expr->name, expr->params, std::move(lambda_return)), environment, false, false);
    return function;
}

std::any Interpreter::visitTernaryExpr(const std::shared_ptr<Ternary> &expr) {
    return isTruthy(evaluate(expr->condition)) ? evaluate(expr->true_branch) : evaluate(expr->else_branch);
}

void Interpreter::resolve(const std::shared_ptr<Expr> &expr, uint32_t depth) {
    locals[expr] = depth;
}

std::any Interpreter::lookUpVariable(const Token &name, const std::shared_ptr<Expr> &expr) {
    auto elem_iter = locals.find(expr);
    if (elem_iter != locals.end()) {
        return environment->getAt(elem_iter->second, name.lexeme);
    } else {
        return globals->get(name);
    }
}

std::any Interpreter::visitClassStmt(const std::shared_ptr<Class> &stmt) {
    std::any superclass = std::shared_ptr<SurpherClass>();
    if (stmt->superclass != nullptr) {
        superclass = evaluate(stmt->superclass);
        if (superclass.type() != typeid(std::shared_ptr<SurpherClass>)) {
            throw RuntimeError(stmt->superclass->name, "Superclass must be a class.");
        }
    }

    environment->define(stmt->name.lexeme, {});

    if (stmt->superclass != nullptr) {
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass);
    }

    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> instance_methods;
    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> class_methods;
    for (const auto &i: stmt->instance_methods) {
        auto function = std::make_shared<SurpherFunction>(i, environment, i->name.lexeme == "init", false);
        instance_methods[i->name.lexeme] = function;
    }
    for (const auto &c: stmt->class_methods) {
        auto function = std::make_shared<SurpherFunction>(c, environment, false, false);
        class_methods[c->name.lexeme] = function;
    }

    auto surpher_class = std::make_shared<SurpherClass>(stmt->name.lexeme, instance_methods, class_methods,
                                                        std::any_cast<std::shared_ptr<SurpherClass>>(superclass));

    if (stmt->superclass != nullptr) {
        environment = environment->getEnclosing();
    }

    environment->assign(stmt->name, surpher_class);
    return {};
}

std::any Interpreter::visitGetExpr(const std::shared_ptr<Get> &expr) {
    std::any object = evaluate(expr->object);
    if (object.type() == typeid(std::shared_ptr<SurpherInstance>)) {
        return (std::any_cast<std::shared_ptr<SurpherInstance>>(object))->get(expr->name);
    } else if (object.type() == typeid(std::shared_ptr<SurpherClass>)) {
        auto tmp_instance = (std::any_cast<std::shared_ptr<SurpherClass>>)(object);
        return std::static_pointer_cast<SurpherInstance>(tmp_instance)->get(expr->name);
    }
    throw RuntimeError(expr->name, "Only instances have properties.");
}

std::any Interpreter::visitSetExpr(const std::shared_ptr<Set> &expr) {
    std::any object = evaluate(expr->object);

    if (!(object.type() == typeid(std::shared_ptr<SurpherInstance>))) {
        throw RuntimeError(expr->name, "Only instances have fields.");
    }

    std::any value = evaluate(expr->value);
    (std::any_cast<std::shared_ptr<SurpherInstance>>(object))->set(expr->name, value);
    return value;
}

std::any Interpreter::visitThisExpr(const std::shared_ptr<This> &expr) {
    return lookUpVariable(expr->keyword, expr);
}

std::any Interpreter::visitSuperExpr(const std::shared_ptr<Super> &expr) {
    auto distance = locals[expr];
    auto superclass = std::any_cast<std::shared_ptr<SurpherClass>>(environment->getAt(distance, "super"));

    auto object = std::any_cast<std::shared_ptr<SurpherInstance>>(environment->getAt(distance - 1, "this"));

    auto method = superclass->findInstanceMethod(expr->method.lexeme);
    if (method == nullptr) {
        method = superclass->findClassMethod(expr->method.lexeme);
    }

    if (method == nullptr) {
        throw RuntimeError(expr->method, "Undefined property '" + expr->method.lexeme + "'.");
    }

    return method->bind(object);
}

