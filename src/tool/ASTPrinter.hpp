#ifndef SURPHER_ASTPRINTER_HPP
#define SURPHER_ASTPRINTER_HPP

#include "Expr.hpp"
#include "Stmt.hpp"

struct ASTPrinter : ExprVisitor, StmtVisitor{
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
};

#endif //SURPHER_ASTPRINTER_HPP
