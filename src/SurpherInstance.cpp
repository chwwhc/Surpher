#include <utility>

#include "SurpherInstance.hpp"
#include "SurpherCallable.hpp"
#include "Token.hpp"
#include "Error.hpp"


SurpherInstance::SurpherInstance(std::shared_ptr<SurpherClass> surpher_class) : surpher_class(
        std::move(surpher_class)) {

}

std::string SurpherInstance::SurpherInstanceToString() {
    return surpher_class->name + " instance";
}

std::any SurpherInstance::get(const Token &name) {
    if (fields.find(name.lexeme) != fields.end()) {
        return fields[name.lexeme];
    }

    if (surpher_class != shared_from_this()) {
        auto method = surpher_class->findInstanceMethod(name.lexeme);
        if (method != nullptr) {
            return method->bind(shared_from_this());
        }

        throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
    } else {
        auto method = surpher_class->findClassMethod(name.lexeme);
        if (method != nullptr) {
            return method;
        }

        throw RuntimeError(name, "Undefined class method '" + name.lexeme + "'.");
    }


}

void SurpherInstance::set(const Token &name, const std::any &value) {
    if (surpher_class == shared_from_this()) {
        throw RuntimeError(name, "Cannot set property to a class.");
    }

    fields[name.lexeme] = value;
}
