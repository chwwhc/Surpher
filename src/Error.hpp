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

void runtimeError(const RuntimeError &error);


#endif //CPPSURPHER_ERROR_HPP
