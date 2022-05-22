package SurpherLangMain;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.function.Function;
import java.util.function.Supplier;

import SurpherLangMain.Token.TokenType;

public class Parser {
    private static class ParseError extends RuntimeException {
    }

    private List<Token> aTokens;
    private int aCurrent = 0;

    Parser(List<Token> pTokens) {
        aTokens = pTokens;
    }

    List<Stmt> parse() {
        List<Stmt> stmts = new ArrayList<>();
        while (!isAtEnd.get())
            stmts.add(declaration());
        return stmts;
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
        if (((new ArrayList<>(Arrays.asList(pTypes))).stream().anyMatch(x -> check.apply(x)))) {
            advance.get();
            return true;
        } else {
            return false;
        }
    }

    private Expr parseBinary(Supplier<Expr> pOperand, TokenType... pTypes) {
        Expr lowerExpr = pOperand.get();

        while (match(pTypes)) {
            Token operator = previous.get();
            Expr right = pOperand.get();
            lowerExpr = new Expr.Binary(lowerExpr, operator, right);
        }

        return lowerExpr;
    }

    private Expr expression() {
        return assignment().get();
    }

    private Supplier<Expr> assignment() {
        Expr expr = comma().get();

        if (match(TokenType.SINGLE_EQUAL)) {
            Token equals = previous.get();
            Expr value = assignment().get();

            if (expr instanceof Expr.Variable) {
                Token name = ((Expr.Variable) expr).getName();
                return () -> new Expr.Assign(name, value);
            }

            error(equals, "Invalid assignment target.");
        }
        return () -> expr;
    }

    private Supplier<Expr> comma() {
        return () -> parseBinary(logicalOR(), TokenType.COMMA);
    }

    private Supplier<Expr> logicalOR() {
        return () -> parseBinary(logicalAND(), TokenType.DOUBLE_BAR);
    }

    private Supplier<Expr> logicalAND() {
        return () -> parseBinary(bitWiseOR(), TokenType.DOUBLE_AMPERSAND);
    }

    private Supplier<Expr> bitWiseOR() {
        return () -> parseBinary(bitWiseXOR(), TokenType.SINGLE_BAR);
    }

    private Supplier<Expr> bitWiseXOR() {
        return () -> parseBinary(bitWiseAND(), TokenType.CARET);
    }

    private Supplier<Expr> bitWiseAND() {
        return () -> parseBinary(equality(), TokenType.SINGLE_AMPERSAND);
    }

    private Supplier<Expr> equality() {
        return () -> parseBinary(comparison(),
                new TokenType[] { TokenType.DOUBLE_EQUAL, TokenType.BANG_EQUAL });
    }

    private Supplier<Expr> comparison() {
        return () -> parseBinary(term(),
                new TokenType[] { TokenType.LESS, TokenType.LESS_EQUAL, TokenType.GREATER, TokenType.GREATER_EQUAL });
    }

    private Supplier<Expr> term() {
        return () -> parseBinary(factor(),
                new TokenType[] { TokenType.SINGLE_PLUS, TokenType.MINUS, TokenType.DOUBLE_PLUS });
    }

    private Supplier<Expr> factor() {
        return () -> parseBinary(unary(),
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
        else if (match(TokenType.NUMBER, TokenType.STRING))
            return new Expr.Literal(previous.get().getLiteral());
        else if (match(TokenType.IDENTIFIER))
            return new Expr.Variable(previous.get());
        else if (match(TokenType.LEFT_PAREN)) {
            Expr expr = expression();
            consume(TokenType.RIGHT_PAREN, "Expect ')' after expression.");
            return new Expr.Grouping(expr);
        }
        throw error(peek.get(), "Expect expression.");
    }

    private Stmt statement() {
        if (match(TokenType.PRINT))
            return printStatement();
        return expressionStatement();
    }

    private Stmt printStatement() {
        Expr value = expression();
        consume(TokenType.DOUBLE_SEMICOLON, "Expect ';;' after value.");
        return new Stmt.Print(value);
    }

    private Stmt declaration() {
        try {
            if (match(TokenType.VAR))
                return varDeclaration();
            return statement();
        } catch (ParseError e) {
            synchronize();
            return null;
        }
    }

    private Stmt varDeclaration() {
        Token name = consume(TokenType.IDENTIFIER, "Expect variable name.");

        Expr initializer = null;
        if (match(TokenType.SINGLE_EQUAL)) {
            initializer = expression();
        }

        consume(TokenType.DOUBLE_SEMICOLON, "Expect \";;\" after variable declaration.");
        return new Stmt.Var(name, initializer);
    }

    private Stmt expressionStatement() {
        Expr expr = expression();
        consume(TokenType.DOUBLE_SEMICOLON, "Expect \";;\" after expression.");
        return new Stmt.Expression(expr);
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
