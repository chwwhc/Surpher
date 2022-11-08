#ifndef SURPHER_PARSER_HPP
#define SURPHER_PARSER_HPP

#include <memory>
#include <vector>
#include <functional>
#include <list>

#include "Token.hpp"
#include "Expr.hpp"
#include "Stmt.hpp"

struct ParseError : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};

class Parser
{
    const std::vector<Token> tokens;
    uint32_t lambdaCount = 0;
    uint32_t current = 0;
    std::function<std::shared_ptr<Expr>()> factor = [this]()
    { return parseBinary(unary, STAR, SLASH, PERCENT); };
    std::function<std::shared_ptr<Expr>()> shift = [this]()
    {
        return parseBinary(term, LEFT_SHIFT, RIGHT_SHIFT);
    };
    std::function<std::shared_ptr<Expr>()> term = [this]()
    { return parseBinary(factor, PLUS, MINUS); };
    std::function<std::shared_ptr<Expr>()> comparison = [this]()
    {
        return parseBinary(shift, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL);
    };
    std::function<std::shared_ptr<Expr>()> equality = [this]()
    {
        return parseBinary(comparison, DOUBLE_EQUAL, BANG_EQUAL);
    };
    std::function<std::shared_ptr<Expr>()> bit_wise_and = [this]()
    { return parseBinary(equality, SINGLE_AMPERSAND); };
    std::function<std::shared_ptr<Expr>()> bit_wise_xor = [this]()
    { return parseBinary(bit_wise_and, CARET); };
    std::function<std::shared_ptr<Expr>()> bit_wise_or = [this]()
    { return parseBinary(bit_wise_xor, SINGLE_BAR); };
    std::function<std::shared_ptr<Expr>()> expression = [this]()
    { return assignment(); };
    std::function<std::shared_ptr<Expr>()> unary = [this]()
    {
        if (match(BANG, MINUS))
        {
            Token op = previous();
            std::shared_ptr<Expr> right = unary();
            return std::shared_ptr<Expr>(new Unary(op, right));
        }
        return access();
    };

    std::shared_ptr<Expr> access();

    std::shared_ptr<Expr> array();

    std::shared_ptr<Expr> call();

    std::shared_ptr<Expr> logicalAnd();

    std::shared_ptr<Expr> logicalOr();

    std::shared_ptr<Expr> ternary();

    std::shared_ptr<Expr> pipe();

    std::shared_ptr<Expr> comma();

    std::shared_ptr<Expr> assignment();

    std::list<std::shared_ptr<Stmt>> blockStatement();

    std::shared_ptr<Stmt> declaration();

    std::shared_ptr<Stmt> varDeclaration(bool is_fixed);

    std::shared_ptr<Stmt> ifStatement();

    std::shared_ptr<Stmt> whileStatement();

    std::shared_ptr<Stmt> forStatement();

    std::shared_ptr<Stmt> breakStatement();

    std::shared_ptr<Stmt> expressionStatement();

    std::shared_ptr<Stmt> continueStatement();

    std::shared_ptr<Stmt> printStatement();

    std::shared_ptr<Stmt> haltStatement();

    std::shared_ptr<Stmt> classDeclaration(bool is_fixed);

    std::shared_ptr<Stmt> importStatement();

    std::shared_ptr<Stmt> namespaceDeclaration(bool is_fixed);

    std::shared_ptr<Function> functionStatement(const std::string &type, bool is_sig, bool is_fixed);

    std::shared_ptr<Stmt> statement();

    std::shared_ptr<Stmt> returnStatement();

    void synchronize();

    Token peek(uint32_t offset);

    bool isAtEnd();

    bool check(TokenType type, uint32_t offset);

    Token previous();

    Token anyToken();

    template <typename... T>
    bool match(T... types);

    template <typename... T>
    std::shared_ptr<Expr> parseBinary(const std::function<std::shared_ptr<Expr>()> &operand, T... types);

    std::shared_ptr<Expr> primary();

    std::shared_ptr<Expr> finishCall(const std::shared_ptr<Expr> &callee);

    Token consume(TokenType type, std::string_view message);

    static ParseError error(const Token &token, std::string_view message);

public:
    explicit Parser(std::vector<Token> tokens);

    std::list<std::shared_ptr<Stmt>> parse();
};

#endif // SURPHER_PARSER_HPP
