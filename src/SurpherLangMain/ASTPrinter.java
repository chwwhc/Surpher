package SurpherLangMain;

import SurpherLangMain.Expr.*;
import SurpherLangMain.Token.*;

public class ASTPrinter implements Expr.ExprVisitor<String> {

    public String ASTPrint(Expr pExpr) {
        return pExpr.accept(this);
    }

    /**
     * Helper method to enclose an expression using parentheses
     * 
     * @param pName  name of the expression
     * @param pExprs list of nonterminals/terminals
     * @return
     */
    private String parenthesize(String pName, Expr... pExprs) {
        StringBuilder builder = new StringBuilder();

        builder.append("(").append(pName);
        for (Expr e : pExprs) {
            builder.append(" ");
            builder.append(e.accept(this));
        }
        builder.append(")");

        return builder.toString();
    }

    @Override
    public String visitBinaryExpr(Binary pExpr) {
        return parenthesize(pExpr.aOperator.getLexeme(), pExpr.aLeft, pExpr.aRight);
    }

    @Override
    public String visitGroupingExpr(Grouping pExpr) {
        return parenthesize("group", pExpr.aExpression);
    }

    @Override
    public String visitLiteralExpr(Literal pExpr) {
        return (pExpr.aValue == null) ? "none" : pExpr.aValue.toString();
    }

    @Override
    public String visitUnaryExpr(Unary pExpr) {
        return parenthesize(pExpr.aOperator.getLexeme(), pExpr.aRight);
    }

    public static void main(String[] args) {
        Expr expression = new Expr.Binary(
                new Expr.Unary(
                        new Token(TokenType.MINUS, "^", null, 1),
                        new Expr.Literal(123)),
                new Token(TokenType.STAR, "%", null, 1),
                new Expr.Grouping(
                        new Expr.Literal(45.67)));

        System.out.println(new ASTPrinter().ASTPrint(expression));
    }

}
