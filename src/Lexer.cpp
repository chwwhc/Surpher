#include <string>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"

std::map<std::string, TokenType> keyWords = {
        {"class",    CLASS},
        {"else",     ELSE},
        {"false",    FALSE},
        {"fun",      FUN},
        {"for",      FOR},
        {"if",       IF},
        {"none",     NONE},
        {"return",   RETURN},
        {"super",    SUPER},
        {"this",     THIS},
        {"true",     TRUE},
        {"var",      VAR},
        {"while",    WHILE},
        {"do",       DO},
        {"match",    MATCH},
        {"case",     CASE},
        {"break",    BREAK},
        {"continue", CONTINUE},
        {"newtype",  NEWTYPE},
        {"print",    PRINT}};

Lexer::Lexer(std::string source_code) : source_code(std::move(source_code)) {
}

char Lexer::anyChar() {
    return source_code[current++];
}

bool Lexer::matchNextChar(const char &expected) {
    if (isAtEnd() || source_code[current] != expected) {
        return false;
    }
    current++;
    return true;
}

void Lexer::addToken(const TokenType &type, const std::any &literal) {
    token_list.emplace_back(Token(source_code.substr(start, current - start), literal, type, line));
}

void Lexer::addToken(const TokenType &type) {
    addToken(type, nullptr);
}

char Lexer::lookAHead(const uint32_t &offset) {
    return (isAtEnd() || current + offset >= source_code.size()) ? '\0' : source_code[current + offset];
}

void Lexer::matchString() {
    while (lookAHead(0) != '"' && !isAtEnd()) {
        if (lookAHead(0) == '\n') {
            line++;
        }
        anyChar();
    }

    if (isAtEnd()) {
        ::error(line, "Unterminated string.");
        return;
    }

    anyChar();

    TokenType type = STRING;
    std::any str_literal = source_code.substr(start + 1, current - 2 - start);
    addToken(type, str_literal);
}


void Lexer::matchNumber() {
    while (isDigit(lookAHead(0))) {
        anyChar();
    }

    if (lookAHead(0) == '.' && isDigit(lookAHead(1))) {
        anyChar();
    }

    while (isDigit(lookAHead(0))) {
        anyChar();
    }

    TokenType type = NUMBER;
    std::any num_literal = std::stod(source_code.substr(start, current - start));
    addToken(type, num_literal);
}


void Lexer::matchIdentifierOrReserved() {
    while (isAlphaNumeric(lookAHead(0))) {
        anyChar();
    }

    std::string word = source_code.substr(start, current - start);
    TokenType type;
    if (keyWords.find(word) != keyWords.end()) {
        type = keyWords[word];
        addToken(type);
        return;
    }
    type = IDENTIFIER;
    addToken(type);

}

void Lexer::skipComment() {
    uint32_t flag = 1;

    while (flag != 0 && !isAtEnd()) {
        if (matchNextChar('/') && matchNextChar('*')) {
            flag++;
        } else if (matchNextChar('*') && matchNextChar('/')) {
            flag--;
            if (isAtEnd()) {
                return;
            }
        }
        if (isAtEnd()) {
            ::error(line, "Unterminated comment.");
            return;
        } else if (matchNextChar('\n')) {
            line++;
        } else {
            anyChar();
        }
    }
}

void Lexer::scanToken() {
    char next_char = anyChar();
    TokenType type;
    switch (next_char) {
        case '(':
            addToken(LEFT_PAREN);
            break;
        case ')':
            addToken(RIGHT_PAREN);
            break;
        case '{':
            addToken(LEFT_BRACE);
            break;
        case '}':
            addToken(RIGHT_BRACE);
            break;
        case '[':
            addToken(RIGHT_BRACKET);
            break;
        case ']':
            addToken(LEFT_BRACKET);
            break;
        case ',':
            addToken(COMMA);
            break;
        case '.':
            addToken(DOT);
            break;
        case ';':
            addToken(SINGLE_SEMICOLON);
            break;
        case '+':
            addToken(SINGLE_PLUS);
            break;
        case '-':
            addToken(MINUS);
            break;
        case '?':
            addToken(QUESTION);
            break;
        case ':':
            addToken(COLON);
            break;
        case '%':
            addToken(PERCENT);
            break;
        case '*':
            addToken(STAR);
            break;
        case '/':
            if (matchNextChar('\\')) {
                addToken(LAMBDA);
            } else if (matchNextChar('/')) {
                while (!isAtEnd() && !matchNextChar('\n')) {
                    anyChar();
                }
                line++;
            } else if (matchNextChar('*')) {
                skipComment();
            } else {
                addToken(SLASH);
            }
            break;
        case '|':
            if (matchNextChar('|')) {
                addToken(DOUBLE_BAR);
            } else {
                addToken(SINGLE_BAR);
            }
            break;
        case '^':
            addToken(CARET);
            break;
        case '&':
            if (matchNextChar('&')) {
                addToken(DOUBLE_AMPERSAND);
            } else {
                addToken(SINGLE_AMPERSAND);
            }
            break;
        case '!':
            if (matchNextChar('=')) {
                addToken(BANG_EQUAL);
            } else {
                addToken(BANG);
            }
            break;
        case '=':
            if (matchNextChar('=')) {
                addToken(DOUBLE_EQUAL);
            } else {
                addToken(SINGLE_EQUAL);
            }
            break;
        case '>':
            if (matchNextChar('=')) {
                addToken(GREATER_EQUAL);
            } else {
                addToken(GREATER);
            }
            break;
        case '<':
            if (matchNextChar('=')) {
                addToken(LESS_EQUAL);
            } else {
                addToken(LESS);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '"':
            matchString();
            break;
        case '\n':
            line++;
            break;
        default:
            if (isDigit(next_char)) {
                matchNumber();
            } else if (isAlpha(next_char)) {
                matchIdentifierOrReserved();
            } else {
                ::error(current, "invalid character");
            }
    }
}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    TokenType type = EOF_TOKEN;
    token_list.emplace_back(Token("", nullptr, type, line));
    return token_list;
}

bool Lexer::isAtEnd() {
    return current >= source_code.size();
}

bool Lexer::isDigit(const char &c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(const char &c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool Lexer::isAlphaNumeric(const char &c) {
    return isDigit(c) || isAlpha(c);
}





