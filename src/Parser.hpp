#ifndef CPPSURPHER_PARSER_HPP
#define CPPSURPHER_PARSER_HPP

#include <vector>
#include <functional>

#include "Error.hpp"
#include "Token.hpp"
#include "Expr.hpp"

class Parser{
struct ParseError: public std::runtime_error{
    using std::runtime_error::runtime_error;
};


    const std::vector<Token> tokens;
    uint32_t current = 0;
    std::function<Token()> peek = [this](){return tokens[current];};
    std::function<bool()> is_at_end = [this](){return peek().token_type == EOF_TOKEN;};
    std::function<bool(const TokenType)> check = [&, this](auto type){ return !is_at_end() && peek().token_type == type;};
    std::function<Token()> previous = [this](){return tokens[current - 1];};
    std::function<Token()> any_token = [this](){if(!is_at_end()){
        current++;
    }
    return previous();};

    template<typename... T>
    bool match(T... types);

    template<typename... T>
    std::shared_ptr<Expr> parseBinary(const std::function<std::shared_ptr<Expr>()>& operand, T... types);
    std::shared_ptr<Expr> primary();
    std::function<std::shared_ptr<Expr>()> unary = [this](){
        if(match(BANG, MINUS)){
            Token op = previous();
            std::shared_ptr<Expr> right = unary();
            return std::shared_ptr<Expr>(new Unary(op, right));
        }
        return primary();
    };
    std::function<std::shared_ptr<Expr>()> factor = [this](){return parseBinary(unary, STAR, SLASH, PERCENT);};
    std::function<std::shared_ptr<Expr>()> term = [this](){return parseBinary(factor, SINGLE_PLUS, MINUS);};
    std::function<std::shared_ptr<Expr>()> comparison = [this](){return parseBinary(term, LESS, LESS_EQUAL, GREATER, GREATER_EQUAL);};
    std::function<std::shared_ptr<Expr>()> equality = [this](){return parseBinary(comparison, DOUBLE_EQUAL, BANG_EQUAL);};
    std::function<std::shared_ptr<Expr>()> bit_wise_and = [this](){return parseBinary(equality, SINGLE_AMPERSAND);};
    std::function<std::shared_ptr<Expr>()> bit_wise_xor = [this](){return parseBinary(bit_wise_and, CARET);};
    std::function<std::shared_ptr<Expr>()> bit_wise_or = [this](){return parseBinary(bit_wise_xor, SINGLE_BAR);};
    std::function<std::shared_ptr<Expr>()> logical_and = [this](){return parseBinary(bit_wise_or, DOUBLE_AMPERSAND);};
    std::function<std::shared_ptr<Expr>()> logical_or = [this](){return parseBinary(logical_and, DOUBLE_BAR);};
    std::shared_ptr<Expr> expression();
    Token consume(TokenType type, std::string_view message);
    static ParseError error(const Token& token, std::string_view message);


public:
    explicit Parser(std::vector<Token> &tokens);
    std::shared_ptr<Expr> parse();
};

#endif //CPPSURPHER_PARSER_HPP
