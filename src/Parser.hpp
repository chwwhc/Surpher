#ifndef CPPSURPHER_PARSER_HPP
#define CPPSURPHER_PARSER_HPP

#include <memory>
#include <vector>
#include <functional>

#include "Error.hpp"
#include "Token.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

namespace {
    uint64_t lambdaCount = 0;
}

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
        return call();
    };
    std::function<std::shared_ptr<Expr>()> call = [this](){
        std::shared_ptr<Expr> expr = primary();

        while(true){
            if(match(LEFT_PAREN)){
                expr = finishCall(expr);
            }else{
                break;
            }
        }

        return expr;
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
    std::function<std::shared_ptr<Expr>()> expression = [this]() { return assignment(); };
    std::function<std::shared_ptr<Expr>()> ternary = [this](){
        std::shared_ptr<Expr> condition = logical_or();

        if(match(QUESTION)){
            Token question = previous();
            auto true_branch = ternary();
            if(match(COLON)){
                Token colon = previous();
                auto else_branch = ternary();
                return std::shared_ptr<Expr>(new Ternary{condition, question, true_branch, colon, else_branch});
            }else{
                error(previous(), "Expect ':' for ternary expression.");
            }
            error(question, "Expect '?' for ternary expression");
        }

        return condition;
    };
    std::function<std::shared_ptr<Expr>()> assignment = [this]() {
        std::shared_ptr<Expr> expr = ternary();

        if (match(SINGLE_EQUAL)) {
            Token equals = previous();
            std::shared_ptr<Expr> value = assignment();

            if (auto *var_expr = dynamic_cast<Variable *>(expr.get())) {
                Token name = var_expr->name;
                return std::shared_ptr<Expr>(new Assign{std::move(name), value});
            }
            error(equals, "Invalid assignment target.");
        }

        return expr;
    };

    std::vector<std::shared_ptr<Stmt>> blockStatement();

    std::shared_ptr<Stmt> declaration();

    std::shared_ptr<Stmt> varDeclaration();

    std::shared_ptr<Stmt> ifStatement();

    std::shared_ptr<Stmt> whileStatement();

    std::shared_ptr<Stmt> forStatement();

    std::shared_ptr<Stmt> breakStatement();

    std::shared_ptr<Stmt> expressionStatement();

    std::shared_ptr<Stmt> continueStatement();

    std::shared_ptr<Stmt> printStatement();

    std::shared_ptr<Stmt> functionStatement(const std::string& type);

    std::shared_ptr<Stmt> statement();

    std::shared_ptr<Stmt> returnStatement();

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

    std::shared_ptr<Expr> finishCall(const std::shared_ptr<Expr> &callee);

    Token consume(TokenType type, std::string_view message);

    static ParseError error(const Token &token, std::string_view message);

public:
    explicit Parser(std::vector<Token> &tokens);

    std::vector<std::shared_ptr<Stmt>> parse();
};

#endif //CPPSURPHER_PARSER_HPP
