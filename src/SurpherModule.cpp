#include <utility>

#include "SurpherModule.hpp"
#include "Expr.hpp"
#include "SurpherInstance.hpp"
#include "Error.hpp"


SurpherModule::SurpherModule(std::string name, std::shared_ptr<Environment> module_environment) : name(std::move(name)), module_environment(std::move(module_environment)){

}

std::string SurpherModule::SurpherModuleToString() {
    return "<module " + name + ">";
}

std::any SurpherModule::get(const Token &var_name) {
    return module_environment->get(var_name);
}

void SurpherModule::set(const Token &var_name, const std::any &value) {
    module_environment->assign(var_name, value);
}
