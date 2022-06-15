#include "Error.hpp"

#include <utility>

static void report(const uint32_t &line, const std::string_view &location, const std::string_view &message) {
    std::cerr << "[line " << line << "] Error " << location << ": " << message << std::endl;
    had_error = true;
}

void error(const Token &token, const std::string_view &message) {
    if (token.token_type == EOF_TOKEN) {
        report(token.line, " at EOF", message);
    } else {
        report(token.line, "at '" + token.lexeme + "'", message);
    }
}

void error(const uint32_t &line, const std::string_view &message) {
    report(line, "", message);
}

RuntimeError::RuntimeError(Token token, const std::string_view &message)
        : std::runtime_error{message.data()}, token{std::move(token)} {

}

void runtimeError(const RuntimeError &error) {
    std::cerr << error.what() <<
              "\n[line " << error.token.line << "]\n";
    had_runtime_error = true;
}
