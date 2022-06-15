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
        {"func",     FUN},
        {"for",      FOR},
        {"if",       IF},
        {"none",     NONE},
        {"return",   RETURN},
        {"super",    SUPER},
        {"self",     SELF},
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

Lexer::Lexer(std::string source_code) : source_code(std::move(source_code)){
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

void Lexer::addToken(const TokenType &type, const std::any& literal) {
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

    while(flag != 0 && !isAtEnd()){
        if(matchNextChar('(') && matchNextChar('*')){
            flag++;
        }else if(matchNextChar('*') && matchNextChar(')')){
            flag--;
        }
        if(isAtEnd()){
            ::error(line, "Unterminated comment.");
            return;
        }else{
            anyChar();
        }
    }
}

void Lexer::scanToken() {
    char next_char = anyChar();
    TokenType type;
    switch(next_char){
        case '(':
            if(matchNextChar('*')){
                skipComment();
            }else{
                type = LEFT_PAREN;
                addToken(type);
            }
            break;
        case ')':
            type = RIGHT_PAREN;
            addToken(type);
            break;
        case '{':
            type = LEFT_BRACE;
            addToken(type);
            break;
        case '}':
            type = RIGHT_BRACE;
            addToken(type);
            break;
        case '[':
            type = RIGHT_BRACKET;
            addToken(type);
            break;
        case ']':
            type = LEFT_BRACKET;
            addToken(type);
            break;
        case ',':
            type = COMMA;
            addToken(type);
            break;
        case '.':
            type = DOT;
            addToken(type);
            break;
        case ';':
            type = SINGLE_SEMICOLON;
            addToken(type);
            break;
        case '+':
            type = SINGLE_PLUS;
            addToken(type);
            break;
        case '-':
            type = MINUS;
            addToken(type);
            break;
        case '%':
            type = PERCENT;
            addToken(type);
            break;
        case '*':
            type = STAR;
            addToken(type);
            break;
        case '/':
            type = SLASH;
            addToken(type);
            break;
        case '|':
            if(matchNextChar('|')){
                type = DOUBLE_BAR;
                addToken(type);
            }else{
                type = SINGLE_BAR;
                addToken(type);
            }
            break;
        case '^':
            type = CARET;
            addToken(type);
            break;
        case '&':
            if(matchNextChar('&')){
                type = DOUBLE_AMPERSAND;
                addToken(type);
            }else{
                type = SINGLE_AMPERSAND;
                addToken(type);
            }
            break;
        case '!':
            if(matchNextChar('=')){
                type = BANG_EQUAL;
                addToken(type);
            }else{
                type = BANG;
                addToken(type);
            }
            break;
        case '=':
            if(matchNextChar('=')){
                type = DOUBLE_EQUAL;
                addToken(type);
            }else{
                type = SINGLE_EQUAL;
                addToken(type);
            }
            break;
        case '>':
            if(matchNextChar('=')){
                type = GREATER_EQUAL;
                addToken(type);
            }else{
                type = GREATER;
                addToken(type);
            }
            break;
        case '<':
            if(matchNextChar('=')){
                type = LESS_EQUAL;
                addToken(type);
            }else{
                type = LESS;
                addToken(type);
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
            if(isDigit(next_char)){
                matchNumber();
            }else if(isAlpha(next_char)){
                matchIdentifierOrReserved();
            }else{
                ::error(current, "invalid character");
            }

    }
}

std::vector<Token> Lexer::scanTokens() {
    while(!isAtEnd()){
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
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isAlphaNumeric(const char &c) {
    return isDigit(c) || isAlpha(c);
}





