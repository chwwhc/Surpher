#include <functional>
#include <memory>

#include "Resolver.hpp"
#include "Interpreter.hpp"
#include "Error.hpp"

Resolver::Resolver(Interpreter &interpreter) : interpreter(interpreter)
{
}

void Resolver::resolve(const std::shared_ptr<Stmt> &stmt)
{
    stmt->accept(*this);
}

void Resolver::resolve(const std::list<std::shared_ptr<Stmt>> &statements)
{
    for (const auto &s : statements)
        resolve(s);
}

std::any Resolver::visitBlockStmt(const std::shared_ptr<Block> &stmt)
{
    beginScope();
    resolve(stmt->statements);
    endScope();
    return {};
}

void Resolver::beginScope()
{
    scopes.push({});
}

void Resolver::endScope()
{
    scopes.pop();
}

std::any Resolver::visitVarStmt(const std::shared_ptr<Var> &stmt)
{
    for (const auto &var_init : stmt->var_inits)
    {
        declare(std::get<0>(var_init));
        if (std::get<2>(var_init))
            resolve(std::get<2>(var_init));
        define(std::get<0>(var_init));
    }

    return {};
}

void Resolver::declare(const Token &name)
{
    if (scopes.empty())
        return;

    auto &scope = scopes.top();
    if (scope.find(name.lexeme) != scope.end())
        error(name, "Already a variable with this name in this scope.");

    scope[name.lexeme] = false;
}

void Resolver::define(const Token &name)
{
    if (scopes.empty())
        return;

    scopes.top()[name.lexeme] = true;
}

std::any Resolver::visitVariableExpr(const std::shared_ptr<Variable> &expr)
{
    if (!scopes.empty())
    {
        auto &scope = scopes.top();
        auto elem(scope.find(expr->name.lexeme));
        if (elem != scope.end() && !elem->second)
            error(expr->name, "Can't read local variable in its own initializer.");
    }

    resolveLocal(expr, expr->name);
    return {};
}

void Resolver::transferStack(std::stack<std::unordered_map<std::string, bool>> &aux_stack)
{
    while (!aux_stack.empty())
    {
        scopes.emplace(aux_stack.top());
        aux_stack.pop();
    }
}

void Resolver::resolveLocal(const std::shared_ptr<Expr> &expr, const Token &name)
{
    std::stack<std::unordered_map<std::string, bool>> aux_stack;
    uint32_t scopes_size(scopes.size());
    for (auto i = 0; i < scopes_size; i++)
    {
        if (scopes.top().find(name.lexeme) != scopes.top().end())
        {
            interpreter.resolve(expr, i);
            transferStack(aux_stack);
            return;
        }
        else
        {
            aux_stack.emplace(scopes.top());
            scopes.pop();
        }
    }
    transferStack(aux_stack);
}

void Resolver::resolve(const std::shared_ptr<Expr> &expr)
{
    expr->accept(*this);
}

std::any Resolver::visitAssignExpr(const std::shared_ptr<Assign> &expr)
{
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return {};
}

std::any Resolver::visitPipeExpr(const std::shared_ptr<Pipe> &expr)
{
    resolve(expr->right);
    resolve(expr->left);
    return {};
}

std::any Resolver::visitFunctionStmt(const std::shared_ptr<Function> &stmt)
{
    declare(stmt->name);
    define(stmt->name);

    resolveFunction(stmt, FunctionType::FUNCTION);
    return {};
}

void Resolver::resolveFunction(const std::shared_ptr<Function> &function, FunctionType type)
{
    auto enclosing_function = current_function;
    current_function = type;

    beginScope();
    for (const auto &param : function->params)
    {
        declare(param);
        define(param);
    }
    resolve(function->body);
    endScope();
    current_function = enclosing_function;
}

std::any Resolver::visitExpressionStmt(const std::shared_ptr<Expression> &stmt)
{
    resolve(stmt->expression);
    return {};
}

std::any Resolver::visitIfStmt(const std::shared_ptr<If> &stmt)
{
    resolve(stmt->condition);
    resolve(stmt->true_branch);
    if (stmt->else_branch)
    {
        resolve(stmt->else_branch);
    }
    return {};
}

std::any Resolver::visitPrintStmt(const std::shared_ptr<Print> &stmt)
{
    resolve(stmt->expression);
    return {};
}

std::any Resolver::visitBreakStmt(const std::shared_ptr<Break> &stmt)
{
    return {};
}

std::any Resolver::visitImportStmt(const std::shared_ptr<Import> &stmt)
{
    resolve(stmt->script);
    return {};
}

std::any Resolver::visitContinueStmt(const std::shared_ptr<Continue> &stmt)
{
    return {};
}

std::any Resolver::visitReturnStmt(const std::shared_ptr<Return> &stmt)
{
    if (current_function == FunctionType::NONE)
        error(stmt->keyword, "Can't return from top-level code.");

    if (stmt->value)
    {
        if (current_function == FunctionType::INITIALIZER)
            error(stmt->keyword, "Can't return a value from an initializer.");

        resolve(stmt->value);
    }
    return {};
}

std::any Resolver::visitWhileStmt(const std::shared_ptr<While> &stmt)
{
    resolve(stmt->condition);
    resolve(stmt->body);
    return {};
}

std::any Resolver::visitHaltStmt(const std::shared_ptr<Halt> &stmt)
{
    resolve(stmt->message);
    return {};
}

std::any Resolver::visitBinaryExpr(const std::shared_ptr<Binary> &expr)
{
    resolve(expr->left);
    resolve(expr->right);
    return {};
}

std::any Resolver::visitCallExpr(const std::shared_ptr<Call> &expr)
{
    resolve(expr->callee);

    for (const auto &argument : expr->arguments)
        resolve(argument);

    return {};
}

std::any Resolver::visitGroupExpr(const std::shared_ptr<Group> &expr)
{
    resolve(expr->expr_in);
    return {};
}

std::any Resolver::visitLiteralExpr(const std::shared_ptr<Literal> &expr)
{
    return {};
}

std::any Resolver::visitLogicalExpr(const std::shared_ptr<Logical> &expr)
{
    resolve(expr->right);
    resolve(expr->left);
    return {};
}

std::any Resolver::visitUnaryExpr(const std::shared_ptr<Unary> &expr)
{
    resolve(expr->right);
    return {};
}

std::any Resolver::visitTernaryExpr(const std::shared_ptr<Ternary> &expr)
{
    resolve(expr->condition);
    resolve(expr->else_branch);
    resolve(expr->true_branch);
    return {};
}

std::any Resolver::visitLambdaExpr(const std::shared_ptr<Lambda> &expr)
{
    std::list<std::shared_ptr<Stmt>> lambda_return{std::make_shared<Return>(Token("", {}, RETURN, 1), expr->body)};
    std::shared_ptr<Function> lambda_fun = std::make_shared<Function>(expr->name, expr->params, lambda_return, false, true);
    return visitFunctionStmt(lambda_fun);
}

std::any Resolver::visitNamespaceStmt(const std::shared_ptr<Namespace> &stmt)
{
    declare(stmt->name);
    define(stmt->name);

    beginScope();
    resolve(stmt->statements);
    endScope();

    return {};
}

std::any Resolver::visitClassStmt(const std::shared_ptr<Class> &stmt)
{
    auto enclosing_class = current_class;
    current_class = ClassType::CLASS;

    declare(stmt->name);
    define(stmt->name);

    if (stmt->superclass)
    {
        auto super_class_var(std::dynamic_pointer_cast<Variable>(stmt->superclass));
        if (super_class_var && stmt->name.lexeme == super_class_var->name.lexeme)
        {
            error(super_class_var->name, "Class can't inherit from itself.");
        }
    }

    if (stmt->superclass)
    {
        current_class = ClassType::SUBCLASS;
        resolve(stmt->superclass);
        beginScope();
        scopes.top()["super"] = true;
    }

    beginScope();
    scopes.top()["this"] = true;

    for (const auto &i : stmt->instance_methods)
    {
        if (i->is_sig)
            continue;

        FunctionType declaration = FunctionType::METHOD;
        if (i->name.lexeme == "init")
            declaration = FunctionType::INITIALIZER;

        resolveFunction(i, declaration);
    }

    for (const auto &c : stmt->class_methods)
    {
        if (c->is_sig)
            continue;

        FunctionType declaration = FunctionType::METHOD;
        if (c->name.lexeme == "init")
            error(c->name, "'init' can't be a class method.");

        resolveFunction(c, declaration);
    }

    endScope();

    if (stmt->superclass)
        endScope();

    current_class = enclosing_class;
    return {};
}

std::any Resolver::visitGetExpr(const std::shared_ptr<Get> &expr)
{
    resolve(expr->object);
    return {};
}

std::any Resolver::visitSetExpr(const std::shared_ptr<Set> &expr)
{
    resolve(expr->value);
    resolve(expr->object);
    return {};
}

std::any Resolver::visitThisExpr(const std::shared_ptr<This> &expr)
{
    if (current_class == ClassType::NONE)
    {
        error(expr->keyword, "Can't use 'this' outside of a class.");
        return {};
    }

    resolveLocal(expr, expr->keyword);
    return {};
}

std::any Resolver::visitSuperExpr(const std::shared_ptr<Super> &expr)
{
    if (current_class == ClassType::NONE)
    {
        error(expr->keyword, "Can't use 'super' outside of a class.");
    }
    else if (current_class != ClassType::SUBCLASS)
    {
        error(expr->keyword, "Can't use 'super' in a class without superclass.");
    }

    resolveLocal(expr, expr->keyword);
    return {};
}

std::any Resolver::visitArrayExpr(const std::shared_ptr<Array> &expr)
{
    std::for_each(expr->expr_vector.begin(), expr->expr_vector.end(), [this](const auto &entry)
                  { resolve(entry); });
    if (expr->dynamic_size != nullptr)
        resolve(expr->dynamic_size);

    return {};
}

std::any Resolver::visitAccessExpr(const std::shared_ptr<Access> &expr)
{
    resolve(expr->arr_name);
    resolve(expr->index);

    return {};
}

std::any Resolver::visitArraySetExpr(const std::shared_ptr<ArraySet> &expr)
{
    resolve(expr->assignee);
    resolve(expr->value);

    return {};
}

std::any Resolver::visitCommaExpr(const std::shared_ptr<Comma> &expr)
{
    for (const auto &e : expr->expressions)
        resolve(e);

    return {};
}
