#include "SurpherCallable.hpp"

#include <utility>
#include "Interpreter.hpp"

SurpherFunction::SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure) : declaration(std::move(declaration)), closure(std::move(closure)) {

}

std::any SurpherFunction::call(Interpreter &interpreter, const std::vector<std::any> &arguments) {
    std::shared_ptr<Environment> environment = std::make_shared<Environment>(closure);
    for (size_t i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }
    try {
        interpreter.executeBlock(declaration->body, environment);
    } catch (ReturnError &returnVal) {
        return returnVal.value;
    }

    return {};
}

uint32_t SurpherFunction::arity() {
    return declaration->params.size();
}

std::string SurpherFunction::SurpherCallableToString() {
    return "<fn "s + declaration->name.lexeme + ">"s;
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
