package SurpherLangMain;

import java.util.Arrays;
import java.util.List;
import java.util.function.BiPredicate;
import java.util.function.Function;
import java.util.function.Predicate;

import SurpherLangMain.Expr.Assign;
import SurpherLangMain.Expr.Binary;
import SurpherLangMain.Expr.Grouping;
import SurpherLangMain.Expr.Literal;
import SurpherLangMain.Expr.Unary;
import SurpherLangMain.Expr.Variable;
import SurpherLangMain.Stmt.Expression;
import SurpherLangMain.Stmt.Print;
import SurpherLangMain.Stmt.Var;

public class Interpreter implements Expr.Visitor<Object>, Stmt.Visitor<Void> {
    private Environment aEnvironment = Environment.getEnvironment();

    private Predicate<Object> isTruthy = pObject -> {
        if (pObject == null)
            return false;
        if (pObject instanceof Boolean)
            return (Boolean) pObject;
        return true;
    };
    private BiPredicate<Object, Object> isEqual = (pObject1, pObject2) -> {
        if (pObject1 == null && pObject2 == null)
            return true;
        if (pObject1 == null)
            return false;
        return pObject1.equals(pObject2);
    };
    private Function<Object, String> stringify = pObject -> {
        if (pObject == null)
            return "none";
        if (pObject instanceof Double) {
            String val = pObject.toString();
            if (val.endsWith(".0")) {
                val = val.substring(0, val.length() - 2);
            }
            return val;
        }
        return pObject.toString();
    };

    private Object evaluate(Expr pExpr) {
        return pExpr.accept(this);
    }

    void interpret(List<Stmt> statements) {
        try {
            statements.stream().forEach(stmt -> stmt.accept(this));
        } catch (RuntimeError error) {
            JSurpher.runtimeError(error);
        }
    }

    @Override
    public Object visitBinaryExpr(Binary pExpr) {
        Object left = evaluate(pExpr.getLeft());
        Object right = evaluate(pExpr.getRight());

        return switch (pExpr.getOperator().getType()) {
            case COMMA -> {
                yield right;
            }
            case MINUS -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left - (double) right;
            }
            case SLASH -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                checkZero(pExpr.getOperator(), (double) right);
                yield (double) left / (double) right;
            }
            case STAR -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left * (double) right;
            }
            case SINGLE_PLUS -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left + (double) right;
            }
            case DOUBLE_PLUS -> {
                checkOperandTypes(pExpr.getOperator(), String.class, "String", left, right);
                yield (String) left + (String) right;
            }
            case CARET -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) (Math.round((double) left) ^ Math.round((double) right));
            }
            case PERCENT -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                checkZero(pExpr.getOperator(), (double) right);
                yield (double) (Math.round((double) left) % Math.round((double) right));
            }
            case SINGLE_AMPERSAND -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) (Math.round((double) left) & Math.round((double) right));
            }
            case SINGLE_BAR -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) (Math.round((double) left) | Math.round((double) right));
            }
            case GREATER -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left > (double) right;
            }
            case GREATER_EQUAL -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left >= (double) right;
            }
            case LESS -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left < (double) right;
            }
            case LESS_EQUAL -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", left, right);
                yield (double) left <= (double) right;
            }
            case BANG_EQUAL -> isEqual.negate().test(left, right);
            case DOUBLE_EQUAL -> isEqual.test(left, right);
            default -> throw new IllegalArgumentException("Unexpected value: " + pExpr.getOperator().getType());
        };
    }

    @Override
    public Object visitGroupingExpr(Grouping pExpr) {
        return evaluate(pExpr.getExpression());
    }

    @Override
    public Object visitLiteralExpr(Literal pExpr) {
        return pExpr.getValue();
    }

    @Override
    public Object visitUnaryExpr(Unary pExpr) {
        Object right = evaluate(pExpr.getRight());

        return switch (pExpr.getOperator().getType()) {
            case BANG -> {
                yield isTruthy.negate().test(right);
            }
            case MINUS -> {
                checkOperandTypes(pExpr.getOperator(), Double.class, "Double", right);
                yield -(Double) right;
            }
            default -> throw new IllegalArgumentException("Unexpected value: " + pExpr.getOperator().getType());
        };
    }

    private void checkOperandTypes(Token pOperator, Class<?> pTargetType, String pTargetTypeName, Object... pOperands) {
        if (Arrays.asList(pOperands).stream().allMatch(operand -> operand.getClass() == pTargetType))
            return;
        throw new RuntimeError(pOperator, "Operand(s) must be a " + pTargetTypeName + ".");
    }

    private void checkZero(Token pOperator, Double... pOperands) {
        if (Arrays.asList(pOperands).stream().allMatch(operand -> operand == 0)) {
            throw new RuntimeError(pOperator, "Denominator must be non-zero.");
        }
        return;
    }

    @Override
    public Void visitExpressionStmt(Expression pStmt) {
        evaluate(pStmt.getExpression());
        return null;
    }

    @Override
    public Void visitVarStmt(Stmt.Var pStmt) {
        // default value of a variable is "none"
        Object value = null;
        if (pStmt.getInitializer() != null)
            value = evaluate(pStmt.getInitializer());

        aEnvironment.define(pStmt.getName().getLexeme(), value);
        return null;
    }

    @Override
    public Object visitVariableExpr(Variable pExpr) {
        return aEnvironment.get(pExpr.getName());
    }

    @Override
    public Void visitPrintStmt(Print pStmt) {
        Object value = evaluate(pStmt.getExpression());
        System.out.println(stringify.apply(value));
        return null;
    }

    @Override
    public Object visitAssignExpr(Assign pExpr) {
        Object value = evaluate(pExpr.getValue());
        aEnvironment.update(pExpr.getName(), value);
        return value;
    }

}
