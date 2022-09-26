#include <utility>
#include <cassert>

#include "Environment.hpp"
#include "Token.hpp"
#include "Error.hpp"

void Environment::assign(const Token &name, const std::any &value) {
    if (var_val_pairs.find(name.lexeme) != var_val_pairs.end()) {
        if(var_val_pairs[name.lexeme].first) throw RuntimeError(name, "Can't modify constant \"" + name.lexeme + "\".");

        var_val_pairs[name.lexeme] = {false, value};
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

std::any Environment::get(const Token &name) {
    if (var_val_pairs.find(name.lexeme) != var_val_pairs.end()) return var_val_pairs[name.lexeme].second;

    if (enclosing != nullptr) return enclosing->get(name);

    throw RuntimeError(name, "Undefined variable \"" + name.lexeme + "\".");
}


void Environment::define(const std::string &var, const std::any& val, const bool is_const) {
    assert(var_val_pairs.find(var) == var_val_pairs.end());

    var_val_pairs[var] = {is_const, val};
}


void Environment::define(const Token &var, std::any val, const bool is_const){
    if (var_val_pairs.find(var.lexeme) != var_val_pairs.end()) {
        if(var_val_pairs[var.lexeme].first) throw RuntimeError(var, "Can't modify constant \"" + var.lexeme + "\".");
    }

    var_val_pairs[var.lexeme] = {is_const, std::move(val)};
}

Environment::Environment() = default;

Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing{std::move(enclosing)} {

}

std::any Environment::getAt(uint32_t distance, const std::string &name) {
    return ancestor(distance)->var_val_pairs[name].second;
}

std::shared_ptr<Environment> Environment::ancestor(uint32_t distance) {
    std::shared_ptr<Environment> environment(shared_from_this());
    for (size_t i = 0; i < distance; i++) environment = environment->enclosing;

    return environment;
}

void Environment::assignAt(uint32_t distance, const Token &name, std::any value) {
    if(ancestor(distance)->var_val_pairs[name.lexeme].first) throw RuntimeError(name, "Can't modify constant \"" + name.lexeme + "\".");
    ancestor(distance)->var_val_pairs[name.lexeme] = {false, std::move(value)};
}

std::shared_ptr<Environment> Environment::getEnclosing() {
    return enclosing;
}

void Environment::erase(const std::string &var) {
        var_val_pairs.erase(var);
}