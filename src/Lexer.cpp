#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>

#include "Lexer.hpp"
#include "Token.hpp"
#include "Error.hpp"

static std::unordered_map<std::string, TokenType> keyWords = {
    {"class", CLASS},
    {"else", ELSE},
    {"false", FALSE},
    {"fun", FUN},
    {"for", FOR},
    {"if", IF},
    {"nil", NIL},
    {"return", RETURN},
    {"super", SUPER},
    {"this", THIS},
    {"true", TRUE},
    {"var", VAR},
    {"while", WHILE},
    {"do", DO},
    {"break", BREAK},
    {"continue", CONTINUE},
    {"print", PRINT},
    {"and", AND},
    {"or", OR},
    {"sig", SIG},
    {"import", IMPORT},
    {"namespace", NAMESPACE},
    {"fixed", FIXED},
    {"halt", HALT},
    {"alloc", ALLOC}};

Lexer::Lexer(std::string source_code) : source_code(std::move(source_code))
{
}

inline char Lexer::anyChar()
{
    return source_code[current++];
}

bool Lexer::matchNextChar(char expected)
{
    if (isAtEnd(0) || source_code[current] != expected)
        return false;
    current++;
    return true;
}

inline void Lexer::addToken(TokenType type, const std::any &literal)
{
    token_list.emplace_back(Token(source_code.substr(start, current - start), literal, type, line));
}

inline void Lexer::addToken(TokenType type)
{
    addToken(type, nullptr);
}

inline char Lexer::lookAHead(uint32_t offset)
{
    return (isAtEnd(0) || current + offset >= source_code.size()) ? '\0' : source_code[current + offset];
}

void Lexer::matchString()
{
    std::ostringstream str_builder;
    while (lookAHead(0) != '"' && !isAtEnd(0))
    {
        if (lookAHead(0) == '\n')
            line++;
        if (lookAHead(0) == '\\' && !isAtEnd(1))
        {
            switch (lookAHead(1))
            {
            case 't':
                str_builder << '\t';
                break;
            case 'n':
                str_builder << '\n';
                break;
            case 'b':
                str_builder << '\b';
                break;
            case 'f':
                str_builder << 'f';
                break;
            case '"':
                str_builder << '"';
                break;
            case '\\':
                str_builder << '\\';
                break;
            default:
                break;
            }
            anyChar();
            anyChar();
            continue;
        }
        str_builder << anyChar();
    }

    if (isAtEnd(0))
    {
        ::error(line, "Unterminated string.");
        return;
    }

    anyChar();

    TokenType type = STRING;
    std::any str_literal = str_builder.str();
    addToken(type, str_literal);
}

void Lexer::matchNumber()
{
    while (isDigit(lookAHead(0)) || lookAHead(0) == '.' || lookAHead(0) == 'e')
        anyChar();

    TokenType type = NUMBER;
    std::any num_literal;
    try
    {
        num_literal = std::stold(source_code.substr(start, current - start));
    }
    catch (const std::exception &e)
    {
        ::error(line, "Number format incorrect.");
    }

    addToken(type, num_literal);
}

void Lexer::matchIdentifierOrReserved()
{
    while (isAlphaNumeric(lookAHead(0)))
        anyChar();

    std::string word = source_code.substr(start, current - start);
    TokenType type;
    if (keyWords.find(word) != keyWords.end())
    {
        type = keyWords[word];
        addToken(type);
        return;
    }
    type = IDENTIFIER;
    addToken(type);
}

void Lexer::skipComment()
{
    uint32_t flag = 1;

    while (flag != 0 && !isAtEnd(0))
    {
        if (matchNextChar('/') && matchNextChar('*'))
        {
            flag++;
        }
        else if (matchNextChar('*') && matchNextChar('/'))
        {
            flag--;
            if (isAtEnd(0))
                return;
        }
        if (isAtEnd(0))
        {
            ::error(line, "Unterminated comment.");
            return;
        }
        else if (matchNextChar('\n'))
        {
            line++;
        }
        else
        {
            anyChar();
        }
    }
}

void Lexer::scanToken()
{
    char next_char = anyChar();
    switch (next_char)
    {
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
        addToken(LEFT_BRACKET);
        break;
    case '@':
        addToken(AT);
        break;
    case ']':
        addToken(RIGHT_BRACKET);
        break;
    case ',':
        addToken(COMMA);
        break;
    case '.':
        if (isDigit(lookAHead(0)))
        {
            matchNumber();
        }
        else
        {
            addToken(DOT);
        }
        break;
    case ';':
        addToken(SINGLE_SEMICOLON);
        break;
    case '+':
        addToken(PLUS);
        break;
    case '-':
        if (matchNextChar('>'))
        {
            addToken(RIGHT_ARROW);
        }
        else
        {
            addToken(MINUS);
        }
        break;
    case '?':
        addToken(QUESTION);
        break;
    case ':':
        addToken(SINGLE_COLON);
        break;
    case '%':
        addToken(PERCENT);
        break;
    case '*':
        addToken(STAR);
        break;
    case '\\':
        addToken(LAMBDA);
        break;
    case '/':
        if (matchNextChar('/'))
        {
            while (!isAtEnd(0) && !matchNextChar('\n'))
            {
                anyChar();
            }
            line++;
        }
        else if (matchNextChar('*'))
        {
            skipComment();
        }
        else
        {
            addToken(SLASH);
        }
        break;
    case '|':
        if (matchNextChar('>'))
        {
            addToken(PIPE);
        }
        else
        {
            addToken(SINGLE_BAR);
        }
        break;
    case '^':
        addToken(CARET);
        break;
    case '&':
        addToken(SINGLE_AMPERSAND);
        break;
    case '!':
        if (matchNextChar('='))
        {
            addToken(BANG_EQUAL);
        }
        else
        {
            addToken(BANG);
        }
        break;
    case '=':
        if (matchNextChar('='))
        {
            addToken(DOUBLE_EQUAL);
        }
        else
        {
            addToken(SINGLE_EQUAL);
        }
        break;
    case '>':
        if (matchNextChar('='))
        {
            addToken(GREATER_EQUAL);
        }
        else if (matchNextChar('>'))
        {
            addToken(RIGHT_SHIFT);
        }
        else
        {
            addToken(GREATER);
        }
        break;
    case '<':
        if (matchNextChar('='))
        {
            addToken(LESS_EQUAL);
        }
        else if (matchNextChar('<'))
        {
            addToken(LEFT_SHIFT);
        }
        else
        {
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
        if (isDigit(next_char))
        {
            matchNumber();
        }
        else if (isAlpha(next_char))
        {
            matchIdentifierOrReserved();
        }
        else
        {
            ::error(current, "invalid character");
        }
    }
}

std::vector<Token> Lexer::scanTokens()
{
    while (!isAtEnd(0))
    {
        start = current;
        scanToken();
    }

    TokenType type = EOF_TOKEN;
    token_list.emplace_back(Token("", nullptr, type, line));
    return token_list;
}

inline bool Lexer::isAtEnd(uint32_t offset)
{
    return current + offset >= source_code.size();
}

inline bool Lexer::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

inline bool Lexer::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

inline bool Lexer::isAlphaNumeric(char c)
{
    return isDigit(c) || isAlpha(c);
}
