#ifndef SURPHER_INTERPRETER_HPP
#define SURPHER_INTERPRETER_HPP

#include <list>
#include "Environment.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

struct SurpherCallable;
struct SurpherInstance;
class SurpherFunction;

class Interpreter : public ExprVisitor, public StmtVisitor {
public:
    const std::shared_ptr<Environment> globals;
private:
    std::list<std::list<std::shared_ptr<Stmt>>> scripts;
    std::shared_ptr<Environment> environment = globals;
    std::unordered_map<std::shared_ptr<Expr>, uint32_t> locals;

    static bool isTruthy(const std::any &val);

    static bool isEqual(const std::any &a, const std::any &b);

    static void checkNumberOperands(const Token &operator_token, const std::vector<std::any> &operands);

    static void checkZero(const Token &operator_token, const std::vector<double> &operands);

    std::any evaluate(const std::shared_ptr<Expr> &expr);

    std::any lookUpVariable(const Token &name, const std::shared_ptr<Expr> &expr);

    void execute(const std::shared_ptr<Stmt> &stmt);

public:
    Interpreter();

    void
    executeBlock(const std::list<std::shared_ptr<Stmt>> &stmts, const std::shared_ptr<Environment> &curr_environment);

    std::any visitLambdaExpr(const std::shared_ptr<Lambda> &expr) override;

    std::any visitBinaryExpr(const std::shared_ptr<Binary> &expr) override;

    std::any visitLogicalExpr(const std::shared_ptr<Logical> &expr) override;

    std::any visitGroupExpr(const std::shared_ptr<Group> &expr) override;

    std::any visitLiteralExpr(const std::shared_ptr<Literal> &expr) override;

    std::any visitUnaryExpr(const std::shared_ptr<Unary> &expr) override;

    std::any visitBlockStmt(const std::shared_ptr<Block> &stmt) override;

    std::any visitExpressionStmt(const std::shared_ptr<Expression> &stmt) override;

    std::any visitPrintStmt(const std::shared_ptr<Print> &stmt) override;

    std::any visitVarStmt(const std::shared_ptr<Var> &stmt) override;

    std::any visitIfStmt(const std::shared_ptr<If> &stmt) override;

    std::any visitBreakStmt(const std::shared_ptr<Break> &stmt) override;

    std::any visitFunctionStmt(const std::shared_ptr<Function> &stmt) override;

    std::any visitContinueStmt(const std::shared_ptr<Continue> &stmt) override;

    std::any visitWhileStmt(const std::shared_ptr<While> &stmt) override;

    std::any visitAssignExpr(const std::shared_ptr<Assign> &expr) override;

    std::any visitVariableExpr(const std::shared_ptr<Variable> &expr) override;

    std::any visitCallExpr(const std::shared_ptr<Call> &expr) override;

    std::any visitTernaryExpr(const std::shared_ptr<Ternary> &expr) override;

    std::any visitReturnStmt(const std::shared_ptr<Return> &stmt) override;

    std::any visitClassStmt(const std::shared_ptr<Class> &stmt) override;

    std::any visitGetExpr(const std::shared_ptr<Get> &expr) override;

    std::any visitSetExpr(const std::shared_ptr<Set> &expr) override;

    std::any visitThisExpr(const std::shared_ptr<This> &expr) override;

    std::any visitSuperExpr(const std::shared_ptr<Super> &expr) override;

    std::any visitImportStmt(const std::shared_ptr<Import> &stmt) override;

    void resolve(const std::shared_ptr<Expr> &expr, uint32_t depth);

    static std::string stringify(const std::any &val);

    void appendScriptBack(const std::list<std::shared_ptr<Stmt>> &script);

    void appendScriptFront(const std::list<std::shared_ptr<Stmt>> &script);

    void interpret();
};

#endif //SURPHER_INTERPRETER_HPP
