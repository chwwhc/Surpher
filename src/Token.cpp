#include <string>
#include <utility>


#include "Token.hpp"

Token::Token(std::string  lexeme, std::any literal, const enum TokenType &token_type,
             const uint32_t &line) : lexeme(std::move(lexeme)), literal(std::move(literal)), token_type(token_type), line(line){
}

std::ostream &operator<<(std::ostream &strm, const Token &tok) {
    TokenType type = tok.token_type;
    std::string literal_str;

    switch (type) {
        case NUMBER:
            literal_str = std::to_string(std::any_cast<double>(tok.literal));
            break;
        case STRING:
            literal_str = std::any_cast<std::string>(tok.literal);
            break;
        case FALSE:
            literal_str = "false";
            break;
        case TRUE:
            literal_str = "true";
            break;
        case IDENTIFIER:
            literal_str = tok.lexeme;
            break;
        default:
            literal_str = "none";
    }
    return strm << "lexical type: " + std::to_string(tok.token_type) + " , lexeme: " + tok.lexeme + ", Literal: " +
                   literal_str;
}