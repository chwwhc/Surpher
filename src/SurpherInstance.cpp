#include <utility>
#include <sstream>

#include "SurpherInstance.hpp"
#include "SurpherCallable.hpp"
#include "Token.hpp"
#include "Error.hpp"


SurpherInstance::SurpherInstance(std::optional<std::shared_ptr<SurpherClass>> surpher_class) : surpher_class(
        std::move(surpher_class)) {
}

std::string SurpherInstance::SurpherInstanceToString() {
    void* self = this;
    std::ostringstream self_addr;
    self_addr << self;
    if(surpher_class.has_value()){
        return "<" + surpher_class.value()->name + " instance>" + " at: " + self_addr.str();
    }else{
        return "<" + dynamic_cast<SurpherClass *>(this)->name + " instance>" + " at: " + self_addr.str();
    }
}

std::any SurpherInstance::get(const Token &name) {
    if (fields.find(name.lexeme) != fields.end()) {
        return fields[name.lexeme];
    }

    if (!dynamic_cast<SurpherClass *>(this)) {
        std::shared_ptr<SurpherFunction> method(surpher_class.value()->findInstanceMethod(name.lexeme));
        if (method != nullptr) return method->bind(shared_from_this());

        throw RuntimeError(name, "Undefined property '" + name.lexeme + "'.");
    } else {
        std::shared_ptr<SurpherFunction> method(dynamic_cast<SurpherClass *>(this)->findClassMethod(name.lexeme));
        if (method != nullptr) return method;

        throw RuntimeError(name, "Undefined class method '" + name.lexeme + "'.");
    }
}

void SurpherInstance::set(const Token &name, const std::any &value) {
    if (dynamic_cast<SurpherClass *>(this)) throw RuntimeError(name, "Cannot set property to a class.");

    fields[name.lexeme] = value;
}
