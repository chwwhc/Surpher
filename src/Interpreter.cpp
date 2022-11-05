#include <cmath>
#include <memory>
#include <numeric>
#include <functional>
#include <utility>
#include <sstream>

#include "Interpreter.hpp"
#include "Error.hpp"
#include "SurpherInstance.hpp"
#include "SurpherCallable.hpp"
#include "SurpherNamespace.hpp"

using namespace std::string_literals;

std::any Interpreter::visitLiteralExpr(const std::shared_ptr<Literal> &expr)
{
    return expr->value;
}

std::any Interpreter::visitGroupExpr(const std::shared_ptr<Group> &expr)
{
    return evaluate(expr->expr_in);
}

std::any Interpreter::visitUnaryExpr(const std::shared_ptr<Unary> &expr)
{
    std::any right(evaluate(expr->right));

    switch (expr->op.token_type)
    {
    case MINUS:
        checkNumberOperands(expr->op, {right});
        if (right.type() == typeid(double))
        {
            return -std::any_cast<double>(right);
        }
        else
        {
            return -std::any_cast<int64_t>(right);
        }
    case BANG:
        return !isTruthy(right);
    default:
        return {};
    }
}

std::any Interpreter::visitBinaryExpr(const std::shared_ptr<Binary> &expr)
{
    std::any left(evaluate(expr->left)), right(evaluate(expr->right));

    switch (expr->op.token_type)
    {
    case MINUS:
        checkNumberOperands(expr->op, {left, right});
        return std::any_cast<double>(left) -
               std::any_cast<double>(right);
    case SLASH:
        checkNumberOperands(expr->op, {left, right});
        if (std::any_cast<double>(right) == 0)
            throw RuntimeError(expr->op, "Denominator cannot be 0.");
        return std::any_cast<double>(left) /
               std::any_cast<double>(right);
    case STAR:
        checkNumberOperands(expr->op, {left, right});
        return std::any_cast<double>(left) *
               std::any_cast<double>(right);
    case SINGLE_PLUS:
    {
        if (left.type() == typeid(double) && right.type() == typeid(double))
        {
            return std::any_cast<double>(left) +
                   std::any_cast<double>(right);
        }
        else if (left.type() == typeid(std::string) || right.type() == typeid(std::string))
        {
            return stringify(left) +
                   stringify(right);
        }
        else
        {
            throw RuntimeError(expr->op, "Operands must be two numbers or two strings.");
        }
    }
    case CARET:
        checkNumberOperands(expr->op, {left, right});
        return (int64_t)std::any_cast<double>(left) ^ (int64_t)std::any_cast<double>(right);
    case PERCENT:
        checkNumberOperands(expr->op, {left, right});
        if (std::any_cast<double>(right) == 0)
            throw RuntimeError(expr->op, "Denominator cannot be 0.");
        return std::fmod(std::any_cast<double>(left), std::any_cast<double>(right));
    case SINGLE_AMPERSAND:
        checkNumberOperands(expr->op, {left, right});
        return (int64_t)std::round(std::any_cast<double>(left)) &
               (int64_t)std::round(std::any_cast<double>(right));
    case SINGLE_BAR:
        checkNumberOperands(expr->op, {left, right});
        return (int64_t)std::round(std::any_cast<double>(left)) |
               (int64_t)std::round(std::any_cast<double>(right));
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

void Interpreter::interpret()
{
    if (scripts.empty())
        return;

    auto curr_script{scripts.front()};
    scripts.pop_front();

    while (!curr_script.empty())
    {
        std::shared_ptr<Stmt> curr_stmt{curr_script.front()};
        curr_script.pop_front();
        try
        {
            execute(curr_stmt);
        }
        catch (RuntimeError &e)
        {
            runtimeError(e);
        }
        catch (BreakError &e)
        {
            breakError(e);
        }
        catch (ContinueError &e)
        {
            continueError(e);
        }
        catch (ImportError &e)
        {
            appendScriptBack(curr_script);
            throw ImportError(std::move(e));
        }
    }
}

std::any Interpreter::visitExpressionStmt(const std::shared_ptr<Expression> &stmt)
{
    evaluate(stmt->expression);
    return {};
}

std::any Interpreter::visitPrintStmt(const std::shared_ptr<Print> &stmt)
{
    std::any value(evaluate(stmt->expression));
    std::cout << stringify(value) << std::endl;
    return {};
}

std::any Interpreter::visitBlockStmt(const std::shared_ptr<Block> &stmt)
{
    executeBlock(stmt->statements, std::make_shared<Environment>(this->environment));
    return {};
}

std::any Interpreter::visitVarStmt(const std::shared_ptr<Var> &stmt)
{
    std::any value;
    if (stmt->initializer.has_value())
        value = evaluate(stmt->initializer.value());

    environment->define(stmt->name, value, stmt->is_fixed);
    return {};
}

std::any Interpreter::visitVariableExpr(const std::shared_ptr<Variable> &expr)
{
    return lookUpVariable(expr->name, expr);
}

std::any Interpreter::visitAssignExpr(const std::shared_ptr<Assign> &expr)
{
    std::any value(evaluate(expr->value));

    auto elem_iter(locals.find(expr));
    if (elem_iter != locals.end())
    {
        environment->assignAt(elem_iter->second, expr->name, value);
    }
    else
    {
        globals->assign(expr->name, value);
    }

    return value;
}

std::any Interpreter::evaluate(const std::shared_ptr<Expr> &expr)
{
    return expr->accept(*this);
}

void Interpreter::execute(const std::shared_ptr<Stmt> &stmt)
{
    stmt->accept(*this);
}

void Interpreter::executeBlock(const std::list<std::shared_ptr<Stmt>> &stmts,
                               const std::shared_ptr<Environment> &curr_environment)
{
    auto previous_environment(environment);
    try
    {
        environment = curr_environment;
        for (const std::shared_ptr<Stmt> &s : stmts)
            execute(s);
    }
    catch (...)
    {
        this->environment = previous_environment;
        throw;
    }
    this->environment = previous_environment;
}

bool Interpreter::isTruthy(const std::any &value)
{
    if (value.type() == typeid(nullptr))
        return false;
    if (value.type() == typeid(bool))
        return std::any_cast<bool>(value);
    return true;
}

bool Interpreter::isEqual(const std::any &a, const std::any &b)
{
    if (a.type() == typeid(nullptr) && b.type() == typeid(nullptr))
        return true;
    if (a.type() == typeid(nullptr))
        return false;
    if (a.type() == typeid(std::string) && b.type() == typeid(std::string))
        return std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    if (a.type() == typeid(double) && b.type() == typeid(double))
        return std::any_cast<double>(a) == std::any_cast<double>(b);
    if (a.type() == typeid(bool) && b.type() == typeid(bool))
        return std::any_cast<bool>(a) == std::any_cast<bool>(b);
    return false;
}

std::string Interpreter::stringify(const std::any &value)
{
    if (value.type() == typeid(nullptr))
    {
        return "nil"s;
    }
    else if (value.type() == typeid(double))
    {
        auto double_val(std::any_cast<double>(value));
        std::string num_str(std::to_string(double_val));
        if (floor(double_val) == double_val)
        {
            uint32_t point_index = 0;
            while (point_index < num_str.size() && num_str[point_index] != '.')
            {
                point_index++;
            }
            return num_str.substr(0, point_index);
        }
        return num_str;
    }
    else if (value.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(value);
    }
    else if (value.type() == typeid(bool))
    {
        return std::any_cast<bool>(value) ? "true"s : "false"s;
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherFunction>))
    {
        return (std::any_cast<std::shared_ptr<SurpherFunction>>(value))->SurpherCallableToString();
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        return (std::any_cast<std::shared_ptr<SurpherInstance>>(value))->SurpherInstanceToString();
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherClass>))
    {
        return (std::any_cast<std::shared_ptr<SurpherClass>>(value))->SurpherCallableToString();
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherNamespace>))
    {
        return (std::any_cast<std::shared_ptr<SurpherNamespace>>(value))->SurpherNamespaceToString();
    }
    else if (value.type() == typeid(SurpherArrayPtr))
    {
        auto expr_vector{std::any_cast<SurpherArrayPtr>(value)};
        if (expr_vector->empty())
        {
            return "[]";
        }

        std::ostringstream str_builder;
        str_builder << "[";
        for (const auto &expr : *expr_vector)
            str_builder << stringify(expr) << ", ";
        auto expr_vector_str{str_builder.str()};

        expr_vector_str.pop_back();
        expr_vector_str.pop_back();

        expr_vector_str.push_back(']');
        return expr_vector_str;
    }
    return "Error in stringify: unrecognized literal type."s;
}

void Interpreter::checkNumberOperands(const Token &operator_token, const std::vector<std::any> &operands)
{
    bool result{std::transform_reduce(
        operands.begin(), operands.end(), true,
        [&](const bool &a, const bool &b)
        {
            return a && b;
        },
        [&](const std::any &operand)
        { return operand.type() == typeid(double); })};
    if (result)
        return;
    throw RuntimeError{operator_token, "Operand must be a number."};
}

std::any Interpreter::visitIfStmt(const std::shared_ptr<If> &stmt)
{
    if (isTruthy(evaluate(stmt->condition)))
    {
        execute(stmt->true_branch);
    }
    else if (stmt->else_branch.has_value())
    {
        execute(stmt->else_branch.value());
    }
    return {};
}

std::any Interpreter::visitLogicalExpr(const std::shared_ptr<Logical> &expr)
{
    std::any left(evaluate(expr->left));

    if (expr->op.token_type == OR)
    {
        if (isTruthy(left))
            return left;
    }
    else
    {
        if (!isTruthy(left))
            return left;
    }

    return evaluate(expr->right);
}

std::any Interpreter::visitWhileStmt(const std::shared_ptr<While> &stmt)
{
    while (isTruthy(evaluate(stmt->condition)))
    {
        try
        {
            execute(stmt->body);
        }
        catch (BreakError &e)
        {
            break;
        }
        catch (ContinueError &e)
        {
            continue;
        }
    }
    return {};
}

std::any Interpreter::visitBreakStmt(const std::shared_ptr<Break> &stmt)
{
    throw BreakError(stmt->break_tok, "'break' must be used in loop");
}

std::any Interpreter::visitContinueStmt(const std::shared_ptr<Continue> &stmt)
{
    throw ContinueError(stmt->continue_tok, "'continue' must be used in loop");
}

std::any Interpreter::visitCallExpr(const std::shared_ptr<Call> &expr)
{
    std::any callee(evaluate(expr->callee));

    std::vector<std::any> arguments;
    for (const auto &argument : expr->arguments)
    {
        arguments.emplace_back(evaluate(argument));
    }
    std::shared_ptr<SurpherCallable> callable;

    if (callee.type() == typeid(std::shared_ptr<SurpherFunction>))
    {
        auto fun_callable(std::any_cast<std::shared_ptr<SurpherFunction>>(callee));
        if (fun_callable->is_virtual)
        {
            throw RuntimeError(fun_callable->declaration->name, "Cannot invoke a virtual method.");
        }

        if (arguments.size() > fun_callable->arity())
        {
            throw RuntimeError(expr->paren,
                               "Expected " + std::to_string(fun_callable->arity()) + " arguments but got " +
                                   std::to_string(arguments.size()) + ".");
        }
        else if (arguments.size() < fun_callable->arity())
        {
            std::shared_ptr<Function> partial_fun(std::make_shared<Function>(
                Token("partial" + fun_callable->declaration->name.lexeme, fun_callable->declaration->name.literal,
                      fun_callable->declaration->name.token_type, fun_callable->declaration->name.line),
                std::vector<Token>(fun_callable->declaration->params.begin() + arguments.size(),
                                   fun_callable->declaration->params.end()),
                fun_callable->declaration->body, fun_callable->is_virtual, true));
            for (size_t i = 0; i < arguments.size(); i++)
            {
                fun_callable->closure->define(fun_callable->declaration->params[i], arguments[i], true);
            }
            std::shared_ptr<SurpherFunction> new_fun(std::make_shared<SurpherFunction>(partial_fun, fun_callable->closure,
                                                                                       fun_callable->is_initializer, true));
            return new_fun;
        }
        else
        {
            if (recursion_counter[fun_callable->declaration->name.lexeme] == max_recursion_depth)
            {
                throw RuntimeError(fun_callable->declaration->name, "Maximum recursion depth reached. (depth limit: " + std::to_string(max_recursion_depth) + ")");
            }
            recursion_counter[fun_callable->declaration->name.lexeme]++;

            return fun_callable->call(*this, arguments);
        }
    }
    else if (callee.type() == typeid(std::shared_ptr<SurpherClass>))
    {
        callable = std::any_cast<std::shared_ptr<SurpherClass>>(callee);
    }
    else if (callee.type() == typeid(std::shared_ptr<NativeFunction>))
    {
        auto native_function = static_cast<std::shared_ptr<NativeFunction>>(std::any_cast<std::shared_ptr<NativeFunction>>(callee));
        native_function->paren = expr->paren;
        callable = native_function;
    }
    else
    {
        throw RuntimeError(expr->paren, "Not a callable instance.");
    }

    if (arguments.size() != callable->arity())
    {
        throw RuntimeError(expr->paren, "Expected " + std::to_string(callable->arity()) + " arguments but got " +
                                            std::to_string(arguments.size()) + ".");
    }

    return callable->call(*this, arguments);
}

Interpreter::Interpreter()
{
    glodbalFunctionSetup(*environment);
    environment->define("IO", IO(), true);
    environment->define("Math", Math(), true);
}

std::any Interpreter::visitFunctionStmt(const std::shared_ptr<Function> &stmt)
{
    std::shared_ptr<SurpherFunction> function(std::make_shared<SurpherFunction>(stmt, environment, false, false));
    environment->define(stmt->name, std::move(function), stmt->is_fixed);
    return {};
}

std::any Interpreter::visitReturnStmt(const std::shared_ptr<Return> &stmt)
{
    std::any value;
    if (stmt->value.has_value())
    {
        value = evaluate(stmt->value.value());
    }

    throw ReturnError(value);
}

std::any Interpreter::visitImportStmt(const std::shared_ptr<Import> &stmt)
{
    throw ImportError(std::any_cast<std::string>(evaluate(stmt->script)));
}

std::any Interpreter::visitLambdaExpr(const std::shared_ptr<Lambda> &expr)
{
    std::list<std::shared_ptr<Stmt>> lambda_return;
    lambda_return.emplace_back(std::make_shared<Return>(Token("return", {}, RETURN, expr->name.line), expr->body));

    auto function = std::make_shared<SurpherFunction>(
        std::make_shared<Function>(expr->name, expr->params, std::move(lambda_return), false, true), environment, false,
        false);
    return function;
}

std::any Interpreter::visitTernaryExpr(const std::shared_ptr<Ternary> &expr)
{
    return isTruthy(evaluate(expr->condition)) ? evaluate(expr->true_branch) : evaluate(expr->else_branch);
}

void Interpreter::resolve(const std::shared_ptr<Expr> &expr, uint32_t depth)
{
    locals[expr] = depth;
}

std::any Interpreter::lookUpVariable(const Token &name, const std::shared_ptr<Expr> &expr)
{
    auto elem_iter(locals.find(expr));
    if (elem_iter != locals.end())
    {
        return environment->getAt(elem_iter->second, name.lexeme);
    }
    else
    {
        return globals->get(name);
    }
}

std::any Interpreter::visitNamespaceStmt(const std::shared_ptr<Namespace> &stmt)
{
    auto new_environment(std::make_shared<Environment>(environment));
    executeBlock(stmt->statements, new_environment);
    environment->define(stmt->name, std::make_shared<SurpherNamespace>(stmt->name.lexeme, new_environment), stmt->is_fixed);

    return {};
}

std::any Interpreter::visitClassStmt(const std::shared_ptr<Class> &stmt)
{
    std::any superclass;
    std::shared_ptr<SurpherClass> superclass_cast;
    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> superclass_instance_methods;
    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> superclass_class_methods;

    if (stmt->superclass.has_value())
    {
        superclass = evaluate(stmt->superclass.value());
        if (superclass.type() != typeid(std::shared_ptr<SurpherClass>))
        {
            throw RuntimeError(stmt->name, "Superclass must be a class.");
        }

        superclass_cast = std::any_cast<std::shared_ptr<SurpherClass>>(superclass);
        superclass_class_methods = superclass_cast->class_methods;
        superclass_instance_methods = superclass_cast->instance_methods;
    }

    environment->define(stmt->name, {}, false);

    if (stmt->superclass.has_value())
    {
        environment = std::make_shared<Environment>(environment);
        environment->define("super", superclass, true);
    }

    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> instance_methods;
    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> class_methods;
    for (const auto &i : stmt->instance_methods)
    {
        std::shared_ptr<SurpherFunction> function(std::make_shared<SurpherFunction>(i, environment, i->name.lexeme == "init", false));
        instance_methods[i->name.lexeme] = function;
    }
    for (const auto &c : stmt->class_methods)
    {
        std::shared_ptr<SurpherFunction> function(std::make_shared<SurpherFunction>(c, environment, false, false));
        class_methods[c->name.lexeme] = function;
    }

    std::shared_ptr<SurpherClass> surpher_class(std::make_shared<SurpherClass>(stmt->name.lexeme, instance_methods, class_methods,
                                                                               superclass_cast));

    if (superclass_cast)
    {
        environment = environment->getEnclosing();
    }

    for (const auto &i : superclass_instance_methods)
    {
        if (i.second->is_virtual &&
            (instance_methods.find(i.first) == instance_methods.end() || instance_methods[i.first]->is_virtual))
        {
            environment->erase(stmt->name.lexeme);
            throw RuntimeError(i.second->declaration->name,
                               "Derived class \"" + stmt->name.lexeme + "\" must implement virtual method \"" + i.first +
                                   "\" from super class \"" + superclass_cast->name + "\".");
        }
    }
    for (const auto &c : superclass_class_methods)
    {
        if (c.second->is_virtual &&
            (class_methods.find(c.first) == class_methods.end() || class_methods[c.first]->is_virtual))
        {
            environment->erase(stmt->name.lexeme);
            throw RuntimeError(c.second->declaration->name,
                               "Derived class \"" + stmt->name.lexeme + "\" must implement virtual method \"" + c.first +
                                   "\" from super class \"" + superclass_cast->name + "\".");
        }
    }

    environment->assign(stmt->name, surpher_class);
    environment->setFixed(stmt->name, stmt->is_fixed);
    return {};
}

std::any Interpreter::visitGetExpr(const std::shared_ptr<Get> &expr)
{
    std::any object(evaluate(expr->object));

    if (object.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        return (std::any_cast<std::shared_ptr<SurpherInstance>>(object))->get(expr->name);
    }
    else if (object.type() == typeid(std::shared_ptr<SurpherClass>))
    {
        return std::static_pointer_cast<SurpherInstance>((std::any_cast<std::shared_ptr<SurpherClass>>)(object))->get(expr->name);
    }
    else if (object.type() == typeid(std::shared_ptr<SurpherNamespace>))
    {
        return (std::any_cast<std::shared_ptr<SurpherNamespace>>(object))->get(expr->name);
    }
    throw RuntimeError(expr->name, "Can only get from a module or a class instance.");
}

std::any Interpreter::visitSetExpr(const std::shared_ptr<Set> &expr)
{
    std::any object(evaluate(expr->object));

    if (object.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        std::any value(evaluate(expr->value));
        (std::any_cast<std::shared_ptr<SurpherInstance>>(object))->set(expr->name, value);
        return value;
    }
    else if (object.type() == typeid(std::shared_ptr<SurpherNamespace>))
    {
        std::any value(evaluate(expr->value));
        (std::any_cast<std::shared_ptr<SurpherNamespace>>(object))->set(expr->name, value);
        return value;
    }

    throw RuntimeError(expr->name, "Only instances have fields.");
}

std::any Interpreter::visitThisExpr(const std::shared_ptr<This> &expr)
{
    return lookUpVariable(expr->keyword, expr);
}

std::any Interpreter::visitSuperExpr(const std::shared_ptr<Super> &expr)
{
    uint32_t distance(locals[expr]);
    auto superclass(std::any_cast<std::shared_ptr<SurpherClass>>(environment->getAt(distance, "super")));

    auto object(std::any_cast<std::shared_ptr<SurpherInstance>>(environment->getAt(distance - 1, "this")));

    std::shared_ptr<SurpherFunction> method(superclass->findInstanceMethod(expr->method.lexeme));
    if (!method)
        method = superclass->findClassMethod(expr->method.lexeme);

    if (!method)
        throw RuntimeError(expr->method, "Undefined property \"" + expr->method.lexeme + "\".");

    return method->bind(object);
}

void Interpreter::appendScriptFront(const std::list<std::shared_ptr<Stmt>> &script)
{
    scripts.emplace_front(script);
}

void Interpreter::appendScriptBack(const std::list<std::shared_ptr<Stmt>> &script)
{
    scripts.emplace_back(script);
}

std::any Interpreter::visitArrayExpr(const std::shared_ptr<Array> &expr)
{
    if (expr->dynamic_size)
    {
        auto actual_size{evaluate(expr->dynamic_size)};
        if (actual_size.type() != typeid(double))
        {
            throw RuntimeError(expr->op, "Size for array can only be a number.");
        }
        else if (std::any_cast<double>(actual_size) < 0)
        {
            throw RuntimeError(expr->op, "Size for array cannot be a negative number.");
        }

        auto size_cast{static_cast<uint64_t>((std::any_cast<double>(actual_size)))};
        return std::make_shared<SurpherArray>(size_cast, nullptr);
    }

    SurpherArrayPtr result{std::make_shared<SurpherArray>(expr->expr_vector.size())};
    for (size_t i = 0; i < expr->expr_vector.size(); i++)
    {
        (*result)[i] = evaluate(expr->expr_vector[i]);
    }

    return result;
}

std::any Interpreter::visitAccessExpr(const std::shared_ptr<Access> &expr)
{
    auto index{evaluate(expr->index)}, arr_name{evaluate(expr->arr_name)};
    if (arr_name.type() != typeid(SurpherArrayPtr))
    {
        throw RuntimeError(expr->op, "Access operator can only be applied to an array.");
    }
    else if (index.type() != typeid(double))
    {
        throw RuntimeError(expr->op, "Index for access operator can only be a positive integer.");
    }

    auto index_cast{static_cast<uint64_t>((std::any_cast<double>(index)))};
    auto arr_name_cast{std::any_cast<SurpherArrayPtr>(arr_name)};

    if (arr_name_cast->size() <= index_cast)
    {
        throw RuntimeError(expr->op, "Index-out-of-bound.");
    }

    return (*arr_name_cast)[index_cast];
}

std::any Interpreter::visitArraySetExpr(const std::shared_ptr<ArraySet> &expr)
{
    auto value{evaluate(expr->value)};
    auto assignee{std::static_pointer_cast<Access>(expr->assignee)};
    auto index{evaluate(assignee->index)}, arr_name{evaluate(assignee->arr_name)};
    if (arr_name.type() != typeid(SurpherArrayPtr))
    {
        throw RuntimeError(expr->op, "Access operator can only be applied to an array.");
    }
    else if (index.type() != typeid(double))
    {
        throw RuntimeError(expr->op, "Index for access operator can only be a number.");
    }
    else if (std::any_cast<double>(index) < 0)
    {
        throw RuntimeError(expr->op, "Index cannot be a negative number.");
    }

    auto index_cast{static_cast<uint64_t>((std::any_cast<double>(index)))};
    auto arr_name_cast{std::any_cast<SurpherArrayPtr>(arr_name)};

    if (arr_name_cast->size() <= index_cast)
    {
        throw RuntimeError(expr->op, "Index-out-of-bound.");
    }

    (*arr_name_cast)[index_cast] = value;
    return value;
}
