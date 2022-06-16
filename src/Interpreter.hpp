#ifndef CPPSURPHER_INTERPRETER_HPP
#define CPPSURPHER_INTERPRETER_HPP

#include <functional>
#include <numeric>
#include "Environment.hpp"
#include "Expr.hpp"
#include "Error.hpp"
#include "Stmt.hpp"

using namespace std::string_literals;

class Interpreter : public ExprVisitor, public StmtVisitor {
    std::shared_ptr<Environment> environment{new Environment{}};

    void
    executeBlock(const std::vector<std::shared_ptr<Stmt>> &stmts, const std::shared_ptr<Environment> &curr_environment);

    static bool isTruthy(const std::any &val);

    static bool isEqual(const std::any &a, const std::any &b);

    static std::string stringify(const std::any &val);

    static void checkNumberOperands(const Token &operator_token, const std::vector<std::any> &operands);

    static void checkZero(const Token &operator_token, const std::vector<double> &operands);

    std::any evaluate(const std::shared_ptr<Expr> &expr);

    void execute(const std::shared_ptr<Stmt> &stmt);

public:
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

    std::any visitContinueStmt(const std::shared_ptr<Continue> &stmt) override;

    std::any visitWhileStmt(const std::shared_ptr<While> &stmt) override;

    std::any visitAssignExpr(const std::shared_ptr<Assign> &expr) override;

    std::any visitVariableExpr(const std::shared_ptr<Variable> &expr) override;

    void interpret(const std::vector<std::shared_ptr<Stmt>> &statements);
};

#endif //CPPSURPHER_INTERPRETER_HPP
