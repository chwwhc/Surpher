#include <utility>

#include "Token.hpp"

Token::Token(std::string lexeme, std::any literal, const enum TokenType &token_type,
             const uint32_t &line) : lexeme(std::move(lexeme)), literal(std::move(literal)), token_type(token_type),
                                     line(line)
{
}