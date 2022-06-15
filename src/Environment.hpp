#ifndef SURPHER_ENVIRONMENT_HPP
#define SURPHER_ENVIRONMENT_HPP

#include <unordered_map>
#include <memory>
#include "Token.hpp"
#include "Error.hpp"

class Environment{
    std::unordered_map<std::string, std::any> var_val_pairs;
    std::shared_ptr<Environment> enclosing;
public:
    void define(const std::string &var, std::any val);

    void assign(const Token &name, std::any value);

    std::any get(const Token &name);

    Environment();
    explicit Environment(std::shared_ptr<Environment> enclosing);
};

#endif //SURPHER_ENVIRONMENT_HPP
