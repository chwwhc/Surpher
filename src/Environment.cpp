#include <utility>

#include "Environment.hpp"
#include "Token.hpp"
#include "Error.hpp"

void Environment::assign(const Token &name, std::any value) {
    if (var_val_pairs.find(name.lexeme) != var_val_pairs.end()) {
        var_val_pairs[name.lexeme] = std::move(value);
        return;
    }
    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

std::any Environment::get(const Token &name) {
    if (var_val_pairs.find(name.lexeme) != var_val_pairs.end()) {
        return var_val_pairs[name.lexeme];
    }
    if (enclosing != nullptr) {
        return enclosing->get(name);
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(const std::string &var, std::any val) {
    var_val_pairs[var] = std::move(val);
}

Environment::Environment() = default;

Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing{std::move(enclosing)} {

}

std::any Environment::getAt(uint32_t distance, const std::string &name) {
    return ancestor(distance)->var_val_pairs[name];
}

std::shared_ptr<Environment> Environment::ancestor(uint32_t distance) {
    std::shared_ptr<Environment> environment = shared_from_this();
    for (size_t i = 0; i < distance; i++) {
        environment = environment->enclosing;
    }
    return environment;
}

void Environment::assignAt(uint32_t distance, const Token &name, std::any value) {
    ancestor(distance)->var_val_pairs[name.lexeme] = std::move(value);
}

std::shared_ptr<Environment> Environment::getEnclosing() {
    return enclosing;
}

void Environment::erase(const std::string &var) {
    var_val_pairs.erase(var);
}
