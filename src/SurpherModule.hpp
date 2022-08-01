#ifndef SURPHER_SURPHERMODULE_HPP
#define SURPHER_SURPHERMODULE_HPP

#include <memory>
#include "Environment.hpp"

struct SurpherModule {
    const std::string name;
    const std::shared_ptr<Environment> module_environment;

    SurpherModule(std::string name, std::shared_ptr<Environment> module_environment);

    std::any get(const Token &var_name);

    void set(const Token &var_name, const std::any &value);

    std::string SurpherModuleToString();
};

#endif //SURPHER_SURPHERMODULE_HPP
