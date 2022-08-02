#ifndef SURPHER_SURPHERNAMESPACE_HPP
#define SURPHER_SURPHERNAMESPACE_HPP

#include <memory>
#include "Environment.hpp"

struct SurpherNamespace {
    const std::string name;
    const std::shared_ptr<Environment> module_environment;

    SurpherNamespace(std::string name, std::shared_ptr<Environment> module_environment);

    std::any get(const Token &var_name);

    void set(const Token &var_name, const std::any &value);

    std::string SurpherNamespaceToString();
};

#endif //SURPHER_SURPHERNAMESPACE_HPP
