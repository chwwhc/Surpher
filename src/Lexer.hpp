#ifndef CPPSURPHER_LEXER_HPP
#define CPPSURPHER_LEXER_HPP

#include <string>
#include <vector>
#include <functional>

#include "Token.hpp"

class Lexer {
    const std::string source_code;
    std::vector<Token> token_list;
    uint32_t line = 1;
    uint32_t start = 0;
    uint32_t current = 0;

    const std::function<bool(void)> is_at_end = [&, this]() {return current >= source_code.size();};
    const std::function<bool(const char)> is_digit = [&](auto c) { return c >= '0' && c <= '9'; };
    const std::function<bool(const char)> is_alpha = [&](auto c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); };
    const std::function<bool(const char)> is_alpha_numeric = [&](auto c) { return is_digit(c) || is_alpha(c); };

    void scanToken();

    void skipComment();

    char anyChar();

    char lookAHead(const uint32_t &offset);

    void addToken(const TokenType &type);

    void addToken(const TokenType &type, const std::any& literal);

    bool matchNextChar(const char &expected);

    void matchString();

    void matchNumber();

    void matchIdentifierOrReserved();

public:
    explicit Lexer(std::string source_code);

    ~Lexer();

    std::vector<Token> scanTokens();
};

#endif // CPPSURPHER_LEXER_HPP