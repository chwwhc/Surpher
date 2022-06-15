#include "Environment.hpp"

#include <utility>

void Environment::assign(const Token &name, std::any value) {
    if(var_val_pairs.find(name.lexeme) != var_val_pairs.end()){
        var_val_pairs[name.lexeme] = std::move(value);
        return;
    }
    if(enclosing != nullptr){
        enclosing->assign(name, value);
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

std::any Environment::get(const Token &name) {
    if(var_val_pairs.find(name.lexeme) != var_val_pairs.end()){
        return var_val_pairs[name.lexeme];
    }
    if(enclosing != nullptr){
        return enclosing->get(name);
    }
    throw RuntimeError(name, "Undefined variable '" + name.lexeme + "'.");
}

void Environment::define(const std::string &var, std::any val) {
    var_val_pairs[var] = std::move(val);
}

Environment::Environment() = default;

Environment::Environment(std::shared_ptr<Environment> enclosing) : enclosing{std::move(enclosing)}{

}
