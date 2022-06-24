#ifndef SURPHER_LEXER_HPP
#define SURPHER_LEXER_HPP

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

    void scanToken();

    bool isAtEnd();

    static bool isDigit(const char &c);

    static bool isAlpha(const char &c);

    static bool isAlphaNumeric(const char &c);

    void skipComment();

    char anyChar();

    char lookAHead(const uint32_t &offset);

    void addToken(const TokenType &type);

    void addToken(const TokenType &type, const std::any &literal);

    bool matchNextChar(const char &expected);

    void matchString();

    void matchNumber();

    void matchIdentifierOrReserved();

public:
    explicit Lexer(std::string source_code);

    std::vector<Token> scanTokens();
};

#endif // SURPHER_LEXER_HPP