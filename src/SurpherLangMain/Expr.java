package SurpherLangMain;

import java.util.List;

/**
 * Auto-generated AST file
 */
abstract class Expr {
  abstract <R> R accept(Visitor<R> pVisitor);

   interface Visitor<R> {
       R visitBinaryExpr(Binary pExpr);
       R visitGroupingExpr(Grouping pExpr);
       R visitLiteralExpr(Literal pExpr);
       R visitUnaryExpr(Unary pExpr);
       R visitVariableExpr(Variable pExpr);
       R visitAssignExpr(Assign pExpr);
       }

   static class Binary extends Expr {
      private final Expr aLeft;
      private final Token aOperator;
      private final Expr aRight;

     Binary(Expr pLeft, Token pOperator, Expr pRight) {
       aLeft = pLeft;
       aOperator = pOperator;
       aRight = pRight;
       }

    @Override
    <R> R accept(Visitor<R> pVisitor) {
      return pVisitor.visitBinaryExpr(this);
   }

    Expr getLeft() {
      return aLeft;
   }

    Token getOperator() {
      return aOperator;
   }

    Expr getRight() {
      return aRight;
   }

   }

   static class Grouping extends Expr {
      private final Expr aExpression;

     Grouping(Expr pExpression) {
       aExpression = pExpression;
       }

    @Override
    <R> R accept(Visitor<R> pVisitor) {
      return pVisitor.visitGroupingExpr(this);
   }

    Expr getExpression() {
      return aExpression;
   }

   }

   static class Literal extends Expr {
      private final Object aValue;

     Literal(Object pValue) {
       aValue = pValue;
       }

    @Override
    <R> R accept(Visitor<R> pVisitor) {
      return pVisitor.visitLiteralExpr(this);
   }

    Object getValue() {
      return aValue;
   }

   }

   static class Unary extends Expr {
      private final Token aOperator;
      private final Expr aRight;

     Unary(Token pOperator, Expr pRight) {
       aOperator = pOperator;
       aRight = pRight;
       }

    @Override
    <R> R accept(Visitor<R> pVisitor) {
      return pVisitor.visitUnaryExpr(this);
   }

    Token getOperator() {
      return aOperator;
   }

    Expr getRight() {
      return aRight;
   }

   }

   static class Variable extends Expr {
      private final Token aName;

     Variable(Token pName) {
       aName = pName;
       }

    @Override
    <R> R accept(Visitor<R> pVisitor) {
      return pVisitor.visitVariableExpr(this);
   }

    Token getName() {
      return aName;
   }

   }

   static class Assign extends Expr {
      private final Token aName;
      private final Expr aValue;

     Assign(Token pName, Expr pValue) {
       aName = pName;
       aValue = pValue;
       }

    @Override
    <R> R accept(Visitor<R> pVisitor) {
      return pVisitor.visitAssignExpr(this);
   }

    Token getName() {
      return aName;
   }

    Expr getValue() {
      return aValue;
   }

   }

}
