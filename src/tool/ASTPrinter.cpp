#include <sstream>
#include "ASTPrinter.hpp"
#include "Interpreter.hpp"

//std::stringstream str_builder;

std::any ASTPrinter::visitBlockStmt(const std::shared_ptr<Block> &stmt) {
    std::stringstream str_builder;
    str_builder << "Block (\n";
    for (auto &s: stmt->statements)
        str_builder << "\t" << std::any_cast<std::string>(s->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitExpressionStmt(const std::shared_ptr<Expression> &stmt) {
    std::stringstream str_builder;
    str_builder << "Expr (\n";
    str_builder << "\t" << std::any_cast<std::string>(stmt->expression->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitPrintStmt(const std::shared_ptr<Print> &stmt) {
    std::stringstream str_builder;
    str_builder << "Print (\n";
    str_builder << "\t" << std::any_cast<std::string>(stmt->expression->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitVarStmt(const std::shared_ptr<Var> &stmt) {
    std::stringstream str_builder;
    str_builder << "VarStatement (\n";
    str_builder << "\t" << stmt->name.lexeme << " " << std::any_cast<std::string>(stmt->initializer->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitIfStmt(const std::shared_ptr<If> &stmt) {
    std::stringstream str_builder;
    str_builder << "If (\n";
    str_builder << "\t" << std::any_cast<std::string>(stmt->condition->accept(*this)) << " "
                << std::any_cast<std::string>(stmt->true_branch->accept(*this));
    if (stmt->else_branch.has_value()) {
        str_builder << std::any_cast<std::string>(stmt->else_branch.value()->accept(*this));
    }
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitWhileStmt(const std::shared_ptr<While> &stmt) {
    std::stringstream str_builder;
    str_builder << "While (\n";
    str_builder << "\t" << std::any_cast<std::string>(stmt->condition->accept(*this)) << " "
                << std::any_cast<std::string>(stmt->body->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitBreakStmt(const std::shared_ptr<Break> &stmt) {
    return "Break\n";
}

std::any ASTPrinter::visitContinueStmt(const std::shared_ptr<Continue> &stmt) {
    return "Continue\n";
}

std::any ASTPrinter::visitFunctionStmt(const std::shared_ptr<Function> &stmt) {
    std::stringstream str_builder;
    str_builder << "Function (\n";
    str_builder << "\t" << stmt->name.lexeme << " ";
    for (const auto &param: stmt->params) str_builder << param.lexeme << " ";
    for (const auto &s: stmt->body) str_builder << std::any_cast<std::string>(s->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitReturnStmt(const std::shared_ptr<Return> &stmt) {
    std::stringstream str_builder;
    str_builder << "Return (\n";
    if (stmt->value.has_value()) {
        str_builder << "\t" << std::any_cast<std::string>(stmt->value.value()->accept(*this));
    }
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitClassStmt(const std::shared_ptr<Class> &stmt) {
    std::stringstream str_builder;
    str_builder << "Class (\n";
    str_builder << stmt->name.lexeme << " ";
    if (stmt->superclass.has_value()) {
        str_builder << std::any_cast<std::string>(stmt->superclass.value()->accept(*this));
    }
    for (const auto &i_m: stmt->instance_methods) str_builder << std::any_cast<std::string>(i_m->accept(*this));
    for (const auto &c_m: stmt->class_methods) str_builder << std::any_cast<std::string>(c_m->accept(*this));
    str_builder << "\n\t)";
    return str_builder.str();
}

std::any ASTPrinter::visitBinaryExpr(const std::shared_ptr<Binary> &expr) {
    std::stringstream str_builder;
    str_builder << "Binary ( ";
    str_builder << std::any_cast<std::string>(expr->left->accept(*this)) << " "
                << expr->op.lexeme << " " << std::any_cast<std::string>(expr->right->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitGroupExpr(const std::shared_ptr<Group> &expr) {
    std::stringstream str_builder;
    str_builder << "Group ( ";
    str_builder << std::any_cast<std::string>(expr->expr_in->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitLiteralExpr(const std::shared_ptr<Literal> &expr) {
    std::stringstream str_builder;
    str_builder << "Literal ( ";
    str_builder << Interpreter::stringify(expr->value) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitUnaryExpr(const std::shared_ptr<Unary> &expr) {
    std::stringstream str_builder;
    str_builder << "Unary ( ";
    str_builder << expr->op.lexeme << " " << std::any_cast<std::string>(expr->right->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitAssignExpr(const std::shared_ptr<Assign> &expr) {
    std::stringstream str_builder;
    str_builder << "Assign ( ";
    str_builder << expr->name.lexeme << " " << std::any_cast<std::string>(expr->value->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitVariableExpr(const std::shared_ptr<Variable> &expr) {
    std::stringstream str_builder;
    str_builder << "Variable ( ";
    str_builder << expr->name.lexeme << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitLogicalExpr(const std::shared_ptr<Logical> &expr) {
    std::stringstream str_builder;
    str_builder << "Logical ( ";
    str_builder << std::any_cast<std::string>(expr->left->accept(*this)) << " "
                << expr->op.lexeme << " " << std::any_cast<std::string>(expr->right->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitCallExpr(const std::shared_ptr<Call> &expr) {
    std::stringstream str_builder;
    str_builder << "Call ( ";
    str_builder << std::any_cast<std::string>(expr->callee->accept(*this)) << " ";
    for (const auto &param: expr->arguments) str_builder << std::any_cast<std::string>(param->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitLambdaExpr(const std::shared_ptr<Lambda> &expr) {
    std::stringstream str_builder;
    str_builder << "Lambda ( ";
    str_builder << expr->name.lexeme << " ";
    for (const auto &param: expr->params) str_builder << param.lexeme << " ";
    str_builder << std::any_cast<std::string>(expr->body->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitTernaryExpr(const std::shared_ptr<Ternary> &expr) {
    std::stringstream str_builder;
    str_builder << "IfElseExpr ( ";
    str_builder << std::any_cast<std::string>(expr->condition->accept(*this)) << " ";
    str_builder << std::any_cast<std::string>(expr->true_branch->accept(*this)) << " ";
    str_builder << std::any_cast<std::string>(expr->else_branch->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitGetExpr(const std::shared_ptr<Get> &expr) {
    std::stringstream str_builder;
    str_builder << "Get ( ";
    str_builder << expr->name.lexeme << " ";
    str_builder << std::any_cast<std::string>(expr->object->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitSetExpr(const std::shared_ptr<Set> &expr) {
    std::stringstream str_builder;
    str_builder << "Set ( ";
    str_builder << expr->name.lexeme << " ";
    str_builder << std::any_cast<std::string>(expr->object->accept(*this)) << " ";
    str_builder << ")";
    return str_builder.str();
}

std::any ASTPrinter::visitThisExpr(const std::shared_ptr<This> &expr) {
    return "This ";
}

std::any ASTPrinter::visitSuperExpr(const std::shared_ptr<Super> &expr) {
    return "Super ";
}
