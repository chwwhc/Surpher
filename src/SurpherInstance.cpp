#include "SurpherInstance.hpp"

#include <utility>


SurpherInstance::SurpherInstance(std::shared_ptr<SurpherClass> surpher_class) : surpher_class(std::move(surpher_class)){

}

std::string SurpherInstance::SurpherInstanceToString() {
    return surpher_class->name + " instance";
}

std::any SurpherInstance::get(const Token &name) {
    if(fields.find(name.lexeme) != fields.end()){
        return fields[name.lexeme];
    }

    auto method = surpher_class->findMethod(name.lexeme);
    if(method != nullptr){
        return method->bind(shared_from_this());
    }

    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
}

void SurpherInstance::set(const Token& name, const std::any &value) {
    fields[name.lexeme] = value;
}
