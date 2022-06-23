#ifndef CPPSURPHER_TOKEN_HPP
#define CPPSURPHER_TOKEN_HPP

#include <string>
#include <iostream>
#include <any>

enum TokenType {
    // symbol
    LEFT_PAREN = 0,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COMMA,
    DOT,
    SINGLE_SEMICOLON,
    SINGLE_PLUS,
    MINUS,
    PERCENT,
    STAR,
    SLASH,
    SINGLE_BAR,
    CARET,
    SINGLE_AMPERSAND,
    BANG,
    BANG_EQUAL,
    SINGLE_EQUAL,
    DOUBLE_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    QUESTION,
    COLON,

    // Literal
    IDENTIFIER,
    STRING,
    NUMBER,
    LAMBDA,

    // reserved
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,
    DO,
    MATCH,
    CASE,
    BREAK,
    CONTINUE,
    NEWTYPE,
    PRINT,
    AND,
    OR,

    EOF_TOKEN
};

typedef struct Token {
    const std::string lexeme;
    const std::any literal;
    const TokenType token_type;
    const uint32_t line;

    Token(std::string lexeme, std::any literal, const enum TokenType &token_type, const uint32_t &line);

} Token;

std::ostream &operator<<(std::ostream &strm, const Token &tok);

#endif // CPPSURPHER_TOKEN_HPP