package SurpherLangMain;

import java.util.Collections;
import java.util.List;

/**
 * Auto-generated AST file
 */
abstract class Stmt {
    abstract <R> R accept(Visitor<R> pVisitor);

    interface Visitor<R> {
        R visitExpressionStmt(Expression pStmt);

        R visitVarStmt(Var pStmt);

        R visitPrintStmt(Print pStmt);

        R visitBlockStmt(Block pStmt);
    }

    static class Expression extends Stmt {
        private final Expr aExpression;

        Expression(Expr pExpression) {
            aExpression = pExpression;
        }

        @Override
        <R> R accept(Visitor<R> pVisitor) {
            return pVisitor.visitExpressionStmt(this);
        }

        Expr getExpression() {
            return aExpression;
        }

    }

    static class Var extends Stmt {
        private final Token aName;
        private final Expr aInitializer;

        Var(Token pName, Expr pInitializer) {
            aName = pName;
            aInitializer = pInitializer;
        }

        @Override
        <R> R accept(Visitor<R> pVisitor) {
            return pVisitor.visitVarStmt(this);
        }

        Token getName() {
            return aName;
        }

        Expr getInitializer() {
            return aInitializer;
        }

    }

    static class Print extends Stmt {
        private final Expr aExpression;

        Print(Expr pExpression) {
            aExpression = pExpression;
        }

        @Override
        <R> R accept(Visitor<R> pVisitor) {
            return pVisitor.visitPrintStmt(this);
        }

        Expr getExpression() {
            return aExpression;
        }

    }

    static class Block extends Stmt {
        private final List<Stmt> aStatements;

        Block(List<Stmt> pStatements) {
            aStatements = pStatements;
        }

        @Override
        <R> R accept(Visitor<R> pVisitor) {
            return pVisitor.visitBlockStmt(this);
        }

        List<Stmt> getStatements() {
            return Collections.unmodifiableList(aStatements);
        }

    }

}
