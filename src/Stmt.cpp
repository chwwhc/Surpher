#include "Stmt.hpp"

#include <utility>

Block::Block(std::vector<std::shared_ptr<Stmt>> statements) : statements{std::move(statements)} {}

std::any Block::accept(StmtVisitor &visitor) {
    return visitor.visitBlockStmt(shared_from_this());
}


Expression::Expression(std::shared_ptr<Expr> expression) : expression{std::move(expression)} {}

std::any Expression::accept(StmtVisitor &visitor) {
    return visitor.visitExpressionStmt(shared_from_this());
}


Print::Print(std::shared_ptr<Expr> expression) : expression{std::move(expression)} {}

std::any Print::accept(StmtVisitor &visitor) {
    return visitor.visitPrintStmt(shared_from_this());
}

Var::Var(Token name, std::shared_ptr<Expr> initializer) : name{std::move(name)}, initializer{std::move(initializer)} {}

std::any Var::accept(StmtVisitor &visitor) {
    return visitor.visitVarStmt(shared_from_this());
}


If::If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> then_branch, std::shared_ptr<Stmt> else_branch)
        : condition{std::move(condition)}, then_branch{std::move(then_branch)}, else_branch{std::move(else_branch)} {

}

std::any If::accept(StmtVisitor &visitor) {
    return visitor.visitIfStmt(shared_from_this());
}

While::While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) : condition{std::move(condition)},
                                                                            body{std::move(body)} {

}

std::any While::accept(StmtVisitor &visitor) {
    return visitor.visitWhileStmt(shared_from_this());
}

std::any Break::accept(StmtVisitor &visitor) {
    return visitor.visitBreakStmt(shared_from_this());
}

Break::Break(Token break_tok) : break_tok{std::move(break_tok)}{

}

std::any Continue::accept(StmtVisitor &visitor) {
    return visitor.visitContinueStmt(shared_from_this());
}

Continue::Continue(Token continue_tok) : continue_tok{std::move(continue_tok)}{

}
