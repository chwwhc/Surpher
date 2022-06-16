#ifndef CPPSURPHER_PARSER_HPP
#define CPPSURPHER_PARSER_HPP

#include <memory>
#include <vector>
#include <functional>

#include "Error.hpp"
#include "Token.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


class Parser {
    const std::vector<Token> tokens;
    uint32_t current = 0;
    std::function<std::shared_ptr<Expr>()> unary = [this]() {
        if (match(BANG, MINUS)) {
            Token op = previous();
            std::shared_ptr<Expr> right = unary();
            return std::shared_ptr<Expr>(new Unary{op, right});
        }
        return primary();
    };
    std::function<std::shared_ptr<Expr>()> factor = [this]() { return parseBinary(unary, STAR, SLASH, PERCENT); };
    std::function<std::shared_ptr<Expr>()> term = [this]() { return parseBinary(factor, SINGLE_PLUS, MINUS); };
    std::function<std::shared_ptr<Expr>()> comparison = [this]() {
        return parseBinary(term, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL);
    };
    std::function<std::shared_ptr<Expr>()> equality = [this]() {
        return parseBinary(comparison, DOUBLE_EQUAL, BANG_EQUAL);
    };
    std::function<std::shared_ptr<Expr>()> bit_wise_and = [this]() { return parseBinary(equality, SINGLE_AMPERSAND); };
    std::function<std::shared_ptr<Expr>()> bit_wise_xor = [this]() { return parseBinary(bit_wise_and, CARET); };
    std::function<std::shared_ptr<Expr>()> bit_wise_or = [this]() { return parseBinary(bit_wise_xor, SINGLE_BAR); };
    std::function<std::shared_ptr<Expr>()> logical_and = [this]() {
        std::shared_ptr<Expr> expr = bit_wise_or();

        while(match(DOUBLE_AMPERSAND)){
            Token op = previous();
            std::shared_ptr<Expr> right = bit_wise_or();
            expr = std::make_shared<Logical>(expr, op, right);
        }

        return expr;
    };
    std::function<std::shared_ptr<Expr>()> logical_or = [this]() {
        std::shared_ptr<Expr> expr = logical_and();

        while(match(DOUBLE_BAR)){
            Token op = previous();
            std::shared_ptr<Expr> right = logical_and();
            expr = std::make_shared<Logical>(expr, op, right);
        }

        return expr;
    };
    std::function<std::shared_ptr<Stmt>()> print_statement = [this]() {
        std::shared_ptr<Expr> val = expression();
        consume(SINGLE_SEMICOLON, "Expect ';' after value.");
        return std::make_shared<Print>(val);
    };
    std::function<std::shared_ptr<Stmt>()> expression_statement = [this]() {
        std::shared_ptr<Expr> expr = expression();
        consume(SINGLE_SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<Expression>(expr);
    };
    std::function<std::shared_ptr<Stmt>()> var_declaration = [this]() {
        Token name = consume(IDENTIFIER, "Expect variable name.");

        std::shared_ptr<Expr> initializer;
        if (match(SINGLE_EQUAL)) {
            initializer = expression();
        }
        consume(SINGLE_SEMICOLON, "Expect ';' after variable declaration.");
        return std::make_shared<Var>(name, initializer);
    };
    std::function<std::shared_ptr<Stmt>()> block_statement = [&, this]() {
        std::vector<std::shared_ptr<Stmt>> statements;

        while (!isAtEnd() && !check(RIGHT_BRACE)) {
            statements.emplace_back(declaration());
        }

        consume(RIGHT_BRACE, "Expect '}' after block.");
        return std::make_shared<Block>(statements);
    };
    std::function<std::shared_ptr<Expr>()> expression = [this]() { return assignment(); };

    std::function<std::shared_ptr<Stmt>()> declaration = [this]() {
        try {
            if (match(VAR)) {
                return var_declaration();
            }
            return statement();
        } catch (ParseError &e) {
            synchronize();
            return std::shared_ptr<Stmt>();
        }
    };

    std::function<std::shared_ptr<Expr>()> assignment = [this]() {
        std::shared_ptr<Expr> expr = logical_or();

        if (match(SINGLE_EQUAL)) {
            Token equals = previous();
            std::shared_ptr<Expr> value = assignment();

            if (auto *var_expr = dynamic_cast<Variable *>(expr.get())) {
                Token name = var_expr->name;
                return std::static_pointer_cast<Expr>(std::make_shared<Assign>(std::move(name), value));
            }
            error(equals, "Invalid assignment target.");
        }

        return expr;
    };
    std::function<std::shared_ptr<Stmt>()> if_statement = [this](){
        consume(LEFT_PAREN, "Expect '(' after 'if'.");
        std::shared_ptr<Expr> condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after if condition.");

        std::shared_ptr<Stmt> then_branch = statement();
        std::shared_ptr<Stmt> else_branch;
        if(match(ELSE)){
            else_branch = statement();
        }

        return std::make_shared<If>(condition, then_branch, else_branch);
    };
    std::function<std::shared_ptr<Stmt>()> while_statement = [this](){
        consume(LEFT_PAREN, "Expect '(' after 'while'.");
        std::shared_ptr<Expr> condition = expression();
        consume(RIGHT_PAREN, "Expect ')' after condition.");
        std::shared_ptr<Stmt> body = statement();

        return std::make_shared<While>(While{condition, body});
    };
    std::function<std::shared_ptr<Stmt>()> for_statement = [this](){
        consume(LEFT_PAREN, "Expect '(' after 'for'.");
        std::shared_ptr<Stmt> initializer;
        if(match(VAR)){
            initializer = var_declaration();
        }else if(!match(SINGLE_SEMICOLON)){
            initializer = expression_statement();
        }

        std::shared_ptr<Expr> condition;
        if(!check(SINGLE_SEMICOLON)){
            condition = expression();
        }
        consume(SINGLE_SEMICOLON, "Expect ';' after 'for' condition.");

        std::shared_ptr<Expr> increment;
        if(!check(RIGHT_PAREN)){
            increment = expression();
        }
        consume(RIGHT_PAREN, "Expect ')' after 'for' clauses.");

        std::shared_ptr<Stmt> body = statement();

        if(increment != nullptr){
            body = std::make_shared<Block>(Block{{body, std::make_shared<Expression>(increment)}});
        }
        if(condition == nullptr){
            condition = std::make_shared<Literal>(true);
        }
        body = std::make_shared<While>(While{condition, body});
        if(initializer != nullptr){
            body = std::make_shared<Block>(Block{{initializer, body}});
        }

        return body;
    };

    std::function<std::shared_ptr<Stmt>()> break_statement = [this](){
        auto break_tok = previous();
        consume(SINGLE_SEMICOLON, "Expect ';' after 'break'.");
        return std::make_shared<Break>(break_tok);
    };

    std::function<std::shared_ptr<Stmt>()> continue_statement = [this](){
        auto continue_tok = previous();
        consume(SINGLE_SEMICOLON, "Expect ';' after 'continue'.");
        return std::make_shared<Continue>(continue_tok);
    };

    std::function<std::shared_ptr<Stmt>()> statement = [this]() {
        if (match(PRINT)) {
            return print_statement();
        } else if (match(LEFT_BRACE)) {
            return block_statement();
        }else if(match(IF)){
            return if_statement();
        }else if(match(WHILE)){
            return while_statement();
        }else if(match(FOR)){
            return for_statement();
        }else if(match(BREAK)){
            return break_statement();
        }else if(match(CONTINUE)){
            return continue_statement();
        }
        else {
            return expression_statement();
        }

    };

    void synchronize();

    Token peek();

    bool isAtEnd();

    bool check(const TokenType &type);

    Token previous();

    Token anyToken();

    template<typename... T>
    bool match(T... types);

    template<typename... T>
    std::shared_ptr<Expr> parseBinary(const std::function<std::shared_ptr<Expr>()> &operand, T... types);

    std::shared_ptr<Expr> primary();

    Token consume(TokenType type, std::string_view message);

    static ParseError error(const Token &token, std::string_view message);

public:
    explicit Parser(std::vector<Token> &tokens);

    std::vector<std::shared_ptr<Stmt>> parse();
};

#endif //CPPSURPHER_PARSER_HPP
