#ifndef CPPSURPHER_ERROR_HPP
#define CPPSURPHER_ERROR_HPP

#include <string_view>

#include "Token.hpp"

inline bool had_error = false;
inline bool had_runtime_error = false;

static void report(const uint32_t &line, const std::string_view &location, const std::string_view &message);

void error(const Token &token, const std::string_view &message);

void error(const uint32_t &line, const std::string_view &message);

struct RuntimeError : public std::runtime_error {
    const Token token;

    RuntimeError(Token token, const std::string_view &message);
};

struct BreakError : public std::runtime_error {
    const Token break_tok;

    BreakError(Token token, const std::string_view &message);
};

struct ContinueError : public std::runtime_error {
    const Token continue_tok;

    ContinueError(Token token, const std::string_view &message);
};

struct ReturnError : public std::runtime_error {
    const std::any value;

    explicit ReturnError(std::any value);
};

void runtimeError(const RuntimeError &error);

void continueError(const ContinueError &error);

void breakError(const BreakError &error);


#endif //CPPSURPHER_ERROR_HPP
