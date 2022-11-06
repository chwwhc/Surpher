#ifndef SURPHER_RESOLVER_HPP
#define SURPHER_RESOLVER_HPP

#include <stack>
#include <vector>
#include <unordered_map>
#include <list>
#include "Expr.hpp"
#include "Stmt.hpp"

class Interpreter;

class Resolver : ExprVisitor, StmtVisitor {
    enum class FunctionType {
        NONE = 0,
        FUNCTION,
        METHOD,
        INITIALIZER
    };
    enum class ClassType {
        NONE = 0,
        CLASS,
        SUBCLASS
    };
    std::stack<std::unordered_map<std::string, bool>> scopes;
    Interpreter &interpreter;
    FunctionType current_function = FunctionType::NONE;
    ClassType current_class = ClassType::NONE;

    void resolve(const std::shared_ptr<Expr> &expr);

    void resolve(const std::shared_ptr<Stmt> &stmt);

    void beginScope();

    void endScope();

    void declare(const Token &name);

    void define(const Token &name);

    void resolveLocal(const std::shared_ptr<Expr> &expr, const Token &name);

    void resolveFunction(const std::shared_ptr<Function> &function, FunctionType type);

    void transferStack(std::stack<std::unordered_map<std::string, bool>> &aux_stack);

public:
    std::any visitBlockStmt(const std::shared_ptr<Block> &stmt) override;

    std::any visitExpressionStmt(const std::shared_ptr<Expression> &stmt) override;

    std::any visitPrintStmt(const std::shared_ptr<Print> &stmt) override;

    std::any visitVarStmt(const std::shared_ptr<Var> &stmt) override;

    std::any visitIfStmt(const std::shared_ptr<If> &stmt) override;

    std::any visitWhileStmt(const std::shared_ptr<While> &stmt) override;

    std::any visitBreakStmt(const std::shared_ptr<Break> &stmt) override;

    std::any visitContinueStmt(const std::shared_ptr<Continue> &stmt) override;

    std::any visitFunctionStmt(const std::shared_ptr<Function> &stmt) override;

    std::any visitReturnStmt(const std::shared_ptr<Return> &stmt) override;

    std::any visitClassStmt(const std::shared_ptr<Class> &stmt) override;

    std::any visitImportStmt(const std::shared_ptr<Import> &stmt) override;

    std::any visitNamespaceStmt(const std::shared_ptr<Namespace> &stmt) override;

    std::any visitHaltStmt(const std::shared_ptr<Halt> &stmt) override;

    std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) override;

    std::any visitGroupExpr(const std::shared_ptr<Group> &expr) override;

    std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) override;

    std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) override;

    std::any visitAssignExpr(const std::shared_ptr<Assign> &expr) override;

    std::any visitVariableExpr(const std::shared_ptr<Variable> &expr) override;

    std::any visitLogicalExpr(const std::shared_ptr<Logical> &expr) override;

    std::any visitCallExpr(const std::shared_ptr<Call> &expr) override;

    std::any visitLambdaExpr(const std::shared_ptr<Lambda> &expr) override;

    std::any visitTernaryExpr(const std::shared_ptr<Ternary> &expr) override;

    std::any visitGetExpr(const std::shared_ptr<Get> &expr) override;

    std::any visitSetExpr(const std::shared_ptr<Set> &expr) override;

    std::any visitThisExpr(const std::shared_ptr<This> &expr) override;

    std::any visitSuperExpr(const std::shared_ptr<Super> &expr) override;

    std::any visitArrayExpr(const std::shared_ptr<Array> &expr) override;

    std::any visitAccessExpr(const std::shared_ptr<Access> &expr) override;

    std::any visitArraySetExpr(const std::shared_ptr<ArraySet> &expr) override;

    std::any visitCommaExpr(const std::shared_ptr<Comma> &expr) override;

    std::any visitPipeExpr(const std::shared_ptr<Pipe> &expr) override;

    void resolve(const std::list<std::shared_ptr<Stmt>> &statements);

    explicit Resolver(Interpreter &interpreter);
};


#endif //SURPHER_RESOLVER_HPP
