package SurpherLangMain;

/**
 * Auto-generated AST file
 */
abstract class Expr {
  abstract <R> R accept(ExprVisitor<R> pVisitor);

  interface ExprVisitor<R> {
    R visitBinaryExpr(Binary pExpr);

    R visitGroupingExpr(Grouping pExpr);

    R visitLiteralExpr(Literal pExpr);

    R visitUnaryExpr(Unary pExpr);
  }

  static class Binary extends Expr {
    final Expr aLeft;
    final Token aOperator;
    final Expr aRight;

    Binary(Expr pLeft, Token pOperator, Expr pRight) {
      aLeft = pLeft;
      aOperator = pOperator;
      aRight = pRight;
    }

    @Override
    <R> R accept(ExprVisitor<R> pVisitor) {
      return pVisitor.visitBinaryExpr(this);
    }
  }

  static class Grouping extends Expr {
    final Expr aExpression;

    Grouping(Expr pExpression) {
      aExpression = pExpression;
    }

    @Override
    <R> R accept(ExprVisitor<R> pVisitor) {
      return pVisitor.visitGroupingExpr(this);
    }
  }

  static class Literal extends Expr {
    final Object aValue;

    Literal(Object pValue) {
      aValue = pValue;
    }

    @Override
    <R> R accept(ExprVisitor<R> pVisitor) {
      return pVisitor.visitLiteralExpr(this);
    }
  }

  static class Unary extends Expr {
    final Token aOperator;
    final Expr aRight;

    Unary(Token pOperator, Expr pRight) {
      aOperator = pOperator;
      aRight = pRight;
    }

    @Override
    <R> R accept(ExprVisitor<R> pVisitor) {
      return pVisitor.visitUnaryExpr(this);
    }
  }

}
