#ifndef SURPHER_ENVIRONMENT_HPP
#define SURPHER_ENVIRONMENT_HPP

#include <unordered_map>
#include <memory>
#include <any>

struct Token;

class Environment : public std::enable_shared_from_this<Environment> {
    std::unordered_map<std::string, std::any> var_val_pairs;
    std::shared_ptr<Environment> enclosing;
public:
    std::shared_ptr<Environment> getEnclosing();

    void define(const std::string &var, std::any val);

    void erase(const std::string &var);

    void assign(const Token &name, std::any value);

    void assignAt(uint32_t distance, const Token &name, std::any value);

    std::any get(const Token &name);

    std::any getAt(uint32_t distance, const std::string &name);

    std::shared_ptr<Environment> ancestor(uint32_t distance);

    Environment();

    explicit Environment(std::shared_ptr<Environment> enclosing);
};

#endif //SURPHER_ENVIRONMENT_HPP
