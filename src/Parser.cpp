#include "Parser.hpp"

template<typename... T>
bool Parser::match(T... types) {
    if ((... || check(types))) {
        anyToken();
        return true;
    }
    return false;
}

template<typename... T>
std::shared_ptr<Expr> Parser::parseBinary(const std::function<std::shared_ptr<Expr>()> &operand, T... types) {
    std::shared_ptr<Expr> lower_expr = operand();

    while (match(types...)) {
        Token op = previous();
        std::shared_ptr<Expr> right = operand();
        lower_expr = std::make_shared<Binary>(lower_expr, op, right);
    }

    return lower_expr;
}

std::shared_ptr<Expr> Parser::primary() {
    if (match(FALSE)) {
        return std::shared_ptr<Expr>(new Literal(false));
    } else if (match(TRUE)) {
        return std::shared_ptr<Expr>(new Literal(true));
    } else if (match(NONE)) {
        return std::shared_ptr<Expr>(new Literal(nullptr));
    } else if (match(NUMBER, STRING)) {
        return std::shared_ptr<Expr>(new Literal(previous().literal));
    } else if (match(LEFT_PAREN)) {
        std::shared_ptr<Expr> expr_in = expression();
        consume(RIGHT_PAREN, "Expected right parentheses.");
        return std::shared_ptr<Expr>(new Group(expr_in));
    } else if (match(IDENTIFIER)) {
        return std::shared_ptr<Expr>(new Variable{previous()});
    }
    throw error(peek(), "Expected expression.");
}

Token Parser::consume(TokenType type, std::string_view message) {
    if (check(type)) {
        return anyToken();
    }
    throw error(peek(), message);
}

Parser::Parser(std::vector<Token> &tokens) : tokens(tokens) {

}

ParseError Parser::error(const Token &token, std::string_view message) {
    ::error(token, message);
    return ParseError{""};
}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.emplace_back(declaration());
    }
    return statements;
}

void Parser::synchronize() {
    anyToken();

    while (!isAtEnd()) {
        if (previous().token_type == SINGLE_SEMICOLON) {
            return;
        }
        switch (peek().token_type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            default:
                break;
        }
        anyToken();
    }

}

Token Parser::peek() {
    return tokens[current];
}

bool Parser::isAtEnd() {
    return peek().token_type == EOF_TOKEN;
}

bool Parser::check(const TokenType &type) {
    return !isAtEnd() && peek().token_type == type;
}

Token Parser::previous() {
    return tokens[current - 1];
}

Token Parser::anyToken() {
    if (!isAtEnd()) {
        current++;
    }
    return previous();
}






