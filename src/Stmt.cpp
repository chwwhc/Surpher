#include "Stmt.hpp"

#include <utility>

Block::Block(std::list<std::shared_ptr<Stmt>> statements) : statements{std::move(statements)} {}

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


If::If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> true_branch,
       std::optional<std::shared_ptr<Stmt>> else_branch)
        : condition(std::move(condition)), true_branch(std::move(true_branch)), else_branch(std::move(else_branch)) {

}

std::any If::accept(StmtVisitor &visitor) {
    return visitor.visitIfStmt(shared_from_this());
}

While::While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body) : condition(std::move(condition)),
                                                                            body(std::move(body)) {

}

std::any While::accept(StmtVisitor &visitor) {
    return visitor.visitWhileStmt(shared_from_this());
}

std::any Break::accept(StmtVisitor &visitor) {
    return visitor.visitBreakStmt(shared_from_this());
}

Break::Break(Token break_tok) : break_tok{std::move(break_tok)} {

}

std::any Continue::accept(StmtVisitor &visitor) {
    return visitor.visitContinueStmt(shared_from_this());
}

Continue::Continue(Token continue_tok) : continue_tok{std::move(continue_tok)} {

}

Function::Function(Token name, std::vector<Token> params, std::list<std::shared_ptr<Stmt>> body, bool is_virtual)
        : name(
        std::move(name)), params(std::move(params)), body(std::move(body)), is_virtual(is_virtual) {

}

std::any Function::accept(StmtVisitor &visitor) {
    return visitor.visitFunctionStmt(shared_from_this());
}

Return::Return(Token keyword, std::optional<std::shared_ptr<Expr>> value) : keyword(std::move(keyword)),
                                                                            value(std::move(value)) {

}

std::any Return::accept(StmtVisitor &visitor) {
    return visitor.visitReturnStmt(shared_from_this());
}

Class::Class(Token name, std::vector<std::shared_ptr<Function>> instance_methods,
             std::vector<std::shared_ptr<Function>> class_methods, std::optional<std::shared_ptr<Variable>> superclass)
        : name(
        std::move(name)),
          instance_methods(
                  std::move(
                          instance_methods)),
          class_methods(
                  std::move(
                          class_methods)),
          superclass(
                  std::move(
                          superclass)) {

}

std::any Class::accept(StmtVisitor &visitor) {
    return visitor.visitClassStmt(shared_from_this());
}

Import::Import(std::shared_ptr<Expr> script, std::optional<std::shared_ptr<Expr>> module_name) : script(
        std::move(script)), module_name(std::move(module_name)) {

}

std::any Import::accept(StmtVisitor &visitor) {
    return visitor.visitImportStmt(shared_from_this());
}
