#ifndef SURPHER_LEXER_HPP
#define SURPHER_LEXER_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Token.hpp"

class Lexer
{
    const std::string source_code;
    std::vector<Token> token_list;
    uint32_t line = 1;
    uint32_t start = 0;
    uint32_t current = 0;

    void scanToken();

    inline bool isAtEnd(uint32_t offset);

    inline bool isDigit(char c);

    inline bool isAlpha(char c);

    inline bool isAlphaNumeric(char c);

    void skipComment();

    inline char anyChar();

    inline char lookAHead(uint32_t offset);

    inline void addToken(TokenType type);

    inline void addToken(TokenType type, const std::any &literal);

    bool matchNextChar(char expected);

    void matchString();

    void matchNumber();

    void matchIdentifierOrReserved();

public:
    explicit Lexer(std::string source_code);

    std::vector<Token> scanTokens();
};

#endif // SURPHER_LEXER_HPP