package SurpherLangMain;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;
import java.util.function.Supplier;

import SurpherLangMain.Token.TokenType;;

public class Parser {
    private static class ParseError extends RuntimeException {
    }

    private List<Token> aTokens;
    private int aCurrent = 0;

    Parser(List<Token> pTokens) {
        aTokens = pTokens;
    }

    Expr parse() {
        try {
            return expression();
        } catch (ParseError e) {
            return null;
        }
    }

    private Supplier<Token> peek = () -> aTokens.get(aCurrent);
    private Supplier<Boolean> isAtEnd = () -> peek.get().getType() == TokenType.EOF;
    private Supplier<Token> previous = () -> aCurrent > 0 ? aTokens.get(aCurrent - 1) : null;
    private Function<TokenType, Boolean> check = pType -> isAtEnd.get() ? false
            : peek.get().getType() == pType;
    private Supplier<Token> advance = () -> {
        if (!isAtEnd.get())
            aCurrent++;
        return previous.get();
    };

    private Token consume(TokenType pType, String pMessage) {
        if (check.apply(pType))
            return advance.get();
        throw error(peek.get(), pMessage);
    }

    private ParseError error(Token pToken, String pMessage) {
        JSurpher.error(pToken, pMessage);
        return new ParseError();
    }

    private boolean match(TokenType... pTypes) {
        if (((new ArrayList<>(Arrays.asList(pTypes))).stream().filter(x -> check.apply(x)).count() > 0)) {
            advance.get();
            return true;
        } else {
            return false;
        }
    }

    private Expr LL1RecursiveCaseHelper(Supplier<Expr> pOperand, TokenType... pTypes) {
        Expr lowerExpr = pOperand.get();

        while (match(pTypes)) {
            Token operator = previous.get();
            Expr right = pOperand.get();
            lowerExpr = new Expr.Binary(lowerExpr, operator, right);
        }

        return lowerExpr;
    }

    private Expr expression() {
        return logicalOR().get();
    }

    private Supplier<Expr> logicalOR() {
        return () -> LL1RecursiveCaseHelper(this.logicalAND(), TokenType.DOUBLE_BAR);
    }

    private Supplier<Expr> logicalAND() {
        return () -> LL1RecursiveCaseHelper(this.bitWiseOR(), TokenType.DOUBLE_AMPERSAND);
    }

    private Supplier<Expr> bitWiseOR() {
        return () -> LL1RecursiveCaseHelper(this.bitWiseXOR(), TokenType.SINGLE_BAR);
    }

    private Supplier<Expr> bitWiseXOR() {
        return () -> LL1RecursiveCaseHelper(this.bitWiseAND(), TokenType.CARET);
    }

    private Supplier<Expr> bitWiseAND() {
        return () -> LL1RecursiveCaseHelper(this.equality(), TokenType.SINGLE_AMPERSAND);
    }

    private Supplier<Expr> equality() {
        return () -> LL1RecursiveCaseHelper(this.comparison(),
                new TokenType[] { TokenType.DOUBLE_EQUAL, TokenType.BANG_EQUAL });
    }

    private Supplier<Expr> comparison() {
        return () -> LL1RecursiveCaseHelper(this.term(),
                new TokenType[] { TokenType.LESS, TokenType.LESS_EQUAL, TokenType.GREATER, TokenType.GREATER_EQUAL });
    }

    private Supplier<Expr> term() {
        return () -> LL1RecursiveCaseHelper(this.factor(), new TokenType[] { TokenType.PLUS, TokenType.MINUS });
    }

    private Supplier<Expr> factor() {
        return () -> LL1RecursiveCaseHelper(this.unary(),
                new TokenType[] { TokenType.STAR, TokenType.SLASH, TokenType.PERCENT });
    }

    private Supplier<Expr> unary() {
        if (match(TokenType.BANG, TokenType.MINUS)) {
            Token operator = previous.get();
            Expr right = unary().get();
            return () -> new Expr.Unary(operator, right);
        }
        return () -> primary();
    }

    private Expr primary() {
        if (match(TokenType.FALSE))
            return new Expr.Literal(false);
        else if (match(TokenType.TRUE))
            return new Expr.Literal(true);
        else if (match(TokenType.NONE))
            return new Expr.Literal(null);
        else if (match(TokenType.INTEGER, TokenType.STRING, TokenType.FLOAT))
            return new Expr.Literal(previous.get().getLiteral());
        else if (match(TokenType.LEFT_PAREN)) {
            Expr expr = expression();
            consume(TokenType.RIGHT_PAREN, "Expect ')' after expression.");
            return new Expr.Grouping(expr);
        }
        throw error(peek.get(), "Expect expression.");
    }

    private void synchronize() {
        advance.get();

        while (!isAtEnd.get()) {
            if (previous.get().getType() == TokenType.DOUBLE_SEMICOLON)
                return;

            switch (peek.get().getType()) {
                case CLASS:
                case FUN:
                case VAR:
                case FOR:
                case IF:
                case BREAK:
                case CASE:
                case DO:
                case WHILE:
                case RETURN:
                case NEWTYPE:
                case MATCH:
                    return;
                default:
                    break;
            }

            advance.get();
        }
    }
}
