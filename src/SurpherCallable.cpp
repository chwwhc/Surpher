#include "SurpherCallable.hpp"

#include <utility>
#include "Interpreter.hpp"

SurpherFunction::SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure, bool is_initializer) : declaration(std::move(declaration)), closure(std::move(closure)), is_initializer(is_initializer) {

}

std::any SurpherFunction::call(Interpreter &interpreter, const std::vector<std::any> &arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for (size_t i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }
    try {
        interpreter.executeBlock(declaration->body, environment);
    } catch (ReturnError &returnVal) {
        if(is_initializer){
            return closure->getAt(0, "this");
        }

        return returnVal.value;
    }

    if(is_initializer){
        return closure->getAt(0, "this");
    }
    return {};
}

uint32_t SurpherFunction::arity() {
    return declaration->params.size();
}

std::string SurpherFunction::SurpherCallableToString() {
    return "<fn "s + declaration->name.lexeme + ">"s;
}

std::shared_ptr<SurpherFunction> SurpherFunction::bind(const std::shared_ptr<SurpherInstance> &instance) {
    auto environment = std::make_shared<Environment>(closure);
    environment->define("this", instance);
    return std::make_shared<SurpherFunction>(declaration, environment, is_initializer);
}


SurpherClass::SurpherClass(std::string  name, std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> methods) :name(std::move(name)), methods(std::move(methods)){

}

std::any SurpherClass::call(Interpreter &interpreter, const std::vector<std::any> &arguments) {
    auto instance = std::make_shared<SurpherInstance>(shared_from_this());
    auto initializer = findMethod("init");
    if(initializer != nullptr){
        initializer->bind(instance)->call(interpreter, arguments);
    }

    return instance;
}

uint32_t SurpherClass::arity() {
    auto initializer = findMethod("init");
    if(initializer == nullptr){
        return 0;
    }
    return initializer->arity();
}

std::string SurpherClass::SurpherCallableToString() {
    return name;
}

std::shared_ptr<SurpherFunction> SurpherClass::findMethod(const std::string& methodName) {
    if(methods.find(methodName) != methods.end()){
        return methods[methodName];
    }

    return {};
}

uint32_t Clock::arity() {
    return 0;
}

std::any Clock::call(Interpreter &interpreter, const std::vector<std::any> &arguments) {
    return std::chrono::duration<double>{std::chrono::system_clock::now().time_since_epoch()}.count() / 1000.0;
}

std::string Clock::SurpherCallableToString() {
    return "<native fn>"s;
}
