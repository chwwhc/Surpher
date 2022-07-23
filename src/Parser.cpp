#include "Parser.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"
#include "Error.hpp"

uint32_t Parser::lambdaCount = 0;

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
    std::shared_ptr<Expr> lower_expr(operand());

    while (match(types...)) {
        Token op(previous());
        std::shared_ptr<Expr> right(operand());
        lower_expr = std::make_shared<Binary>(lower_expr, op, right);
    }

    return lower_expr;
}

std::shared_ptr<Expr> Parser::primary() {
    if (match(FALSE)) {
        return std::make_shared<Literal>(false);
    } else if (match(TRUE)) {
        return std::make_shared<Literal>(true);
    } else if (match(NIL)) {
        return std::make_shared<Literal>(nullptr);
    } else if (match(NUMBER, STRING)) {
        return std::make_shared<Literal>(previous().literal);
    } else if (match(LEFT_PAREN)) {
        std::shared_ptr<Expr> expr_in(expression());
        consume(RIGHT_PAREN, "Expected right parentheses.");
        return std::make_shared<Group>(expr_in);
    } else if (match(IDENTIFIER)) {
        return std::make_shared<Variable>(previous());
    } else if (match(LAMBDA)) {
        auto tmp = previous();
        Token lambdaTok("lambda" + std::to_string(lambdaCount++), tmp.literal, tmp.token_type, tmp.line);

        std::vector<Token> params;
        Token param(consume(IDENTIFIER, "Expect a variable after lambda."));
        params.emplace_back(param);

        consume(DOT, "Expect '.' after lambda variable.");

        std::shared_ptr<Expr> body (expression());

        return std::make_shared<Lambda>(lambdaTok, params, body);
    } else if (match(THIS)) {
        return std::make_shared<This>(previous());
    } else if (match(SUPER)) {
        Token keyword(previous());
        consume(DOT, "Expect '.' after 'super'.");
        Token method(consume(IDENTIFIER, "Expect superclass method name."));
        return std::make_shared<Super>(keyword, method);
    }
    throw error(peek(), "Expected expression.");
}

Token Parser::consume(TokenType type, std::string_view message) {
    if (check(type)) return anyToken();
    throw error(peek(), message);
}

Parser::Parser(std::vector<Token> &tokens) : tokens(tokens) {
}

ParseError Parser::error(const Token &token, std::string_view message) {
    ::error(token, message);
    return ParseError{""};
}

std::list<std::shared_ptr<Stmt>> Parser::parse() {
    std::list<std::shared_ptr<Stmt>> statements;
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

std::shared_ptr<Expr> Parser::finishCall(const std::shared_ptr<Expr> &callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(RIGHT_PAREN)) {
        do {
            arguments.emplace_back(expression());
        } while (match(COMMA));
    }

    Token paren(consume(RIGHT_PAREN, "Expect ')' after arguments."));

    return std::make_shared<Call>(callee, paren, arguments);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match(PRINT)) {
        return printStatement();
    } else if (match(LEFT_BRACE)) {
        return std::make_shared<Block>(blockStatement());
    } else if (match(IF)) {
        return ifStatement();
    } else if (match(WHILE)) {
        return whileStatement();
    } else if (match(FOR)) {
        return forStatement();
    } else if (match(BREAK)) {
        return breakStatement();
    } else if (match(CONTINUE)) {
        return continueStatement();
    } else if (match(RETURN)) {
        return returnStatement();
    } else if(match(IMPORT)){
        return importStatement();
    } else {
        return expressionStatement();
    }
}

std::shared_ptr<Function> Parser::functionStatement(const std::string &type, const bool &is_virtual) {
    Token name(consume(IDENTIFIER, "Expect " + type + " name."));
    if(is_virtual){
        consume(LEFT_PAREN, "Expect '(' after declaring a virtual function.");
        consume(RIGHT_PAREN, "Expect ')' after declaring a virtual function.");
        consume(SINGLE_SEMICOLON, "Expect ';' after declaring a virtual function.");
        return std::make_shared<Function>(name, std::vector<Token>(), std::list<std::shared_ptr<Stmt>>(), is_virtual);
    }

    consume(LEFT_PAREN, "Expect '(' after " + type + " name.");
    std::vector<Token> params;
    if (!check(RIGHT_PAREN)) {
        do {
            params.emplace_back(consume(IDENTIFIER, "Expect parameter name."));
        } while (match(COMMA));
    }
    consume(RIGHT_PAREN, "Expect ')' after parameters.");

    consume(LEFT_BRACE, "Expect '{' before " + type + " body.");
    std::list<std::shared_ptr<Stmt>> body(blockStatement());
    return std::make_shared<Function>(name, params, body, is_virtual);
}

std::shared_ptr<Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value;
    if (!check(SINGLE_SEMICOLON)) {
        value = expression();
    }

    consume(SINGLE_SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<Return>(keyword, value);
}

std::shared_ptr<Stmt> Parser::continueStatement() {
    Token continue_tok(previous());
    consume(SINGLE_SEMICOLON, "Expect ';' after 'continue'.");
    return std::make_shared<Continue>(continue_tok);
}

std::shared_ptr<Stmt> Parser::breakStatement() {
    Token break_tok(previous());
    consume(SINGLE_SEMICOLON, "Expect ';' after 'break'.");
    return std::make_shared<Break>(break_tok);
}

std::shared_ptr<Stmt> Parser::forStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'for'.");
    std::shared_ptr<Stmt> initializer;
    if (match(VAR)) {
        initializer = varDeclaration();
    } else if (!match(SINGLE_SEMICOLON)) {
        initializer = expressionStatement();
    }

    std::shared_ptr<Expr> condition;
    if (!check(SINGLE_SEMICOLON)) {
        condition = expression();
    }
    consume(SINGLE_SEMICOLON, "Expect ';' after 'for' condition.");

    std::shared_ptr<Expr> increment;
    if (!check(RIGHT_PAREN)) {
        increment = expression();
    }
    consume(RIGHT_PAREN, "Expect ')' after 'for' clauses.");

    std::shared_ptr<Stmt> body(statement());

    if (increment != nullptr) {
        body = std::make_shared<Block>(Block{{body, std::make_shared<Expression>(increment)}});
    }
    if (condition == nullptr) {
        condition = std::make_shared<Literal>(true);
    }
    body = std::make_shared<While>(While{condition, body});
    if (initializer != nullptr) {
        body = std::make_shared<Block>(Block{{initializer, body}});
    }

    return body;
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition(expression());
    consume(RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body(statement());

    return std::make_shared<While>(While{condition, body});
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition(expression());
    consume(RIGHT_PAREN, "Expect ')' after if condition.");

    std::shared_ptr<Stmt> then_branch(statement());
    std::optional<std::shared_ptr<Stmt>> else_branch(std::nullopt);
    if (match(ELSE)) {
        else_branch.emplace(statement());
    }

    return std::make_shared<If>(condition, then_branch, else_branch);
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match(FUN)) {
            return functionStatement("function", false);
        } else if (match(VAR)) {
                return varDeclaration();
        } else if (match(CLASS)) {
            return classDeclaration();
        }
        return statement();
    } catch (ParseError &e) {
        synchronize();
        return {};
    }
}

std::list<std::shared_ptr<Stmt>> Parser::blockStatement() {
    std::list<std::shared_ptr<Stmt>> statements;

    while (!isAtEnd() && !check(RIGHT_BRACE)) {
        statements.emplace_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name(consume(IDENTIFIER, "Expect variable name."));

    std::shared_ptr<Expr> initializer;
    if (match(SINGLE_EQUAL)) {
        initializer = expression();
    }
    consume(SINGLE_SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, initializer);
}

std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr(expression());
    consume(SINGLE_SEMICOLON, "Expect ';' after expression.");
    return std::make_shared<Expression>(expr);
}

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> value(expression());
    consume(SINGLE_SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(value);
}

std::shared_ptr<Stmt> Parser::classDeclaration() {
    Token name(consume(IDENTIFIER, "Expect class name."));

    std::optional<std::shared_ptr<Variable>> superclass(std::nullopt);
    if (match(LESS)) {
        consume(IDENTIFIER, "Expect superclass name.");
        superclass.emplace(std::make_shared<Variable>(previous()));
    }

    consume(LEFT_BRACE, "Expect '{' before class body.");

    std::vector<std::shared_ptr<Function>> class_methods;
    std::vector<std::shared_ptr<Function>> instance_methods;
    while (!isAtEnd() && !check(RIGHT_BRACE)) {
        bool is_virtual(false);
        if(match(VIRTUAL)){
            is_virtual = true;
        }

        if (match(CLASS)) {
            class_methods.emplace_back(functionStatement("class_method", is_virtual));
        } else {
            instance_methods.emplace_back(functionStatement("instance_method", is_virtual));
        }
    }

    consume(RIGHT_BRACE, "Expect '}' after class body.");

    return std::make_shared<Class>(name, instance_methods, class_methods, superclass);
}

std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr(ternary());

    if (match(SINGLE_EQUAL)) {
        Token equals(previous());
        std::shared_ptr<Expr> value(assignment());

        if (auto *var_expr = dynamic_cast<Variable *>(expr.get())) {
            Token name(var_expr->name);
            return std::make_shared<Assign>(std::move(name), value);
        } else if (auto *get = dynamic_cast<Get *>(expr.get())) {
            return std::make_shared<Set>(get->object, get->name, value);
        }
        error(equals, "Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::ternary() {
    std::shared_ptr<Expr> condition(logicalOr());

    if (match(QUESTION)) {
        Token question(previous());
        std::shared_ptr<Expr> true_branch(ternary());
        if (match(COLON)) {
            Token colon(previous());
            std::shared_ptr<Expr> else_branch(ternary());
            return std::shared_ptr<Expr>(new Ternary{condition, question, true_branch, colon, else_branch});
        } else {
            error(previous(), "Expect ':' for ternary expression.");
        }
        error(question, "Expect '?' for ternary expression");
    }

    return condition;
}

std::shared_ptr<Expr> Parser::logicalOr() {
    std::shared_ptr<Expr> expr(logicalAnd());

    while (match(OR)) {
        Token op(previous());
        std::shared_ptr<Expr> right(logicalAnd());
        expr = std::make_shared<Logical>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::logicalAnd() {
    std::shared_ptr<Expr> expr(bit_wise_or());

    while (match(AND)) {
        Token op(previous());
        std::shared_ptr<Expr> right(bit_wise_or());
        expr = std::make_shared<Logical>(expr, op, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::call() {
    std::shared_ptr<Expr> expr(primary());

    while (true) {
        if (match(LEFT_PAREN)) {
            expr = finishCall(expr);
        } else if (match(DOT)) {
            Token name(consume(IDENTIFIER, "Expect property name after '.'."));
            expr = std::make_shared<Get>(expr, name);
        } else {
            break;
        }
    }

    return expr;
}

std::shared_ptr<Stmt> Parser::importStatement() {
    std::shared_ptr<Expr> path(expression());
    std::optional<std::shared_ptr<Expr>> module_name(std::nullopt);

    if(match(AS)) module_name.emplace(expression());

    consume(SINGLE_SEMICOLON, "Expect ';' after script path.");
    return std::make_shared<Import>(path, module_name);
}






