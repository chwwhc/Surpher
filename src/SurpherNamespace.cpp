#include <utility>
#include <sstream>

#include "SurpherNamespace.hpp"
#include "Expr.hpp"
#include "SurpherInstance.hpp"
#include "Error.hpp"


SurpherNamespace::SurpherNamespace(std::string name, std::shared_ptr<Environment> module_environment) : name(std::move(name)), module_environment(std::move(module_environment)){

}

std::string SurpherNamespace::SurpherNamespaceToString() {
    void* self {this};
    std::ostringstream self_addr;
    self_addr << self;
    return "<namespace " + name + ">" + " at: " + self_addr.str();;
}

std::any SurpherNamespace::get(const Token &var_name) {
    return module_environment->get(var_name);
}

void SurpherNamespace::set(const Token &var_name, const std::any &value) {
    module_environment->assign(var_name, value);
}
