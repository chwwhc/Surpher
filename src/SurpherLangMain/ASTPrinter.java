package SurpherLangMain;

import SurpherLangMain.Expr.*;
import SurpherLangMain.Token.*;

public class ASTPrinter implements Expr.Visitor<String> {

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
        return parenthesize(pExpr.getOperator().getLexeme(), pExpr.getLeft(), pExpr.getRight());
    }

    @Override
    public String visitGroupingExpr(Grouping pExpr) {
        return parenthesize("group", pExpr.getExpression());
    }

    @Override
    public String visitLiteralExpr(Literal pExpr) {
        if (pExpr.getValue() instanceof Double) {
            double val = (double) pExpr.getValue();
            if ((int) val == val) {
                return Integer.toString((int) val);
            } else {
                return Double.toString(val);
            }
        }
        return (pExpr.getValue() == null) ? "none" : pExpr.getValue().toString();
    }

    @Override
    public String visitUnaryExpr(Unary pExpr) {
        return parenthesize(pExpr.getOperator().getLexeme(), pExpr.getRight());
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

    @Override
    public String visitVariableExpr(Variable pExpr) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String visitAssignExpr(Assign pExpr) {
        // TODO Auto-generated method stub
        return null;
    }

}
