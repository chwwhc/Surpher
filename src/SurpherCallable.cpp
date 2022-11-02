#include <utility>
#include <chrono>
#include <sstream>
#include <cmath>

#include "SurpherCallable.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"

using namespace std::string_literals;

SurpherFunction::SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure,
                                 bool is_initializer, bool is_partial) : is_virtual(declaration->is_virtual), declaration(std::move(declaration)),
                                                                         closure(std::move(closure)), is_initializer(is_initializer),
                                                                         is_partial(is_partial)
{
}

std::any SurpherFunction::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto environment{is_partial ? closure : std::make_shared<Environment>(closure)};
    for (size_t i = 0; i < declaration->params.size(); i++)
    {
        environment->define(declaration->params[i], arguments[i], false);
    }
    try
    {
        interpreter.executeBlock(declaration->body, environment);
    }
    catch (ReturnError &returnVal)
    {
        if (is_initializer)
            return closure->getAt(0, "this");

        return returnVal.value;
    }

    if (is_initializer)
        return closure->getAt(0, "this");

    return {};
}

uint32_t SurpherFunction::arity()
{
    return declaration->params.size();
}

std::string SurpherFunction::SurpherCallableToString()
{
    void *self = this;
    std::ostringstream self_addr;
    self_addr << self;
    return "<function "s + declaration->name.lexeme + ">"s + " at: "s + self_addr.str();
}

std::shared_ptr<SurpherFunction> SurpherFunction::bind(const std::shared_ptr<SurpherInstance> &instance)
{
    auto environment = closure;
    environment->define("this", instance, true);
    return std::make_shared<SurpherFunction>(declaration, std::move(environment), is_initializer, is_partial);
}

SurpherClass::SurpherClass(std::string name,
                           std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> instance_methods,
                           std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> class_methods,
                           std::shared_ptr<SurpherClass> superclass) : SurpherInstance(std::nullopt), name(std::move(name)), instance_methods(std::move(instance_methods)),
                                                                       class_methods(std::move(class_methods)),
                                                                       superclass(std::move(superclass))
{
}

std::any SurpherClass::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto instance(std::make_shared<SurpherInstance>(std::make_shared<SurpherClass>(*this)));
    auto initializer(findInstanceMethod("init"));

    if (initializer)
        initializer->bind(instance)->call(interpreter, arguments);

    return instance;
}

uint32_t SurpherClass::arity()
{
    auto initializer(findInstanceMethod("init"));
    if (initializer == nullptr)
        return 0;

    return initializer->arity();
}

std::string SurpherClass::SurpherCallableToString()
{
    void *self = this;
    std::ostringstream self_addr;
    self_addr << self;
    return "<class " + name + ">" + " at: "s + self_addr.str();
}

std::shared_ptr<SurpherFunction> SurpherClass::findInstanceMethod(const std::string &methodName)
{
    if (instance_methods.find(methodName) != instance_methods.end())
        return instance_methods[methodName];

    if (superclass != nullptr)
        return superclass->findInstanceMethod(methodName);

    return {};
}

std::shared_ptr<SurpherFunction> SurpherClass::findClassMethod(const std::string &methodName)
{
    if (class_methods.find(methodName) != class_methods.end())
        return class_methods[methodName];

    if (superclass != nullptr)
        return superclass->findClassMethod(methodName);

    return {};
}

uint32_t NativeFunction::Clock::arity()
{
    return 0;
}

std::any NativeFunction::Clock::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    return std::chrono::duration<double>{std::chrono::system_clock::now().time_since_epoch()}.count();
}

std::string NativeFunction::Clock::SurpherCallableToString()
{
    return {};
}

uint32_t NativeFunction::Sizeof::arity()
{
    return 1;
}

std::any NativeFunction::Sizeof::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() == typeid(std::shared_ptr<SurpherArray>))
    {
        auto value_cast{std::any_cast<std::shared_ptr<SurpherArray>>(value)};
        return static_cast<double>(value_cast->size());
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        auto value_cast{std::any_cast<std::shared_ptr<SurpherInstance>>(value)};
        return std::any_cast<std::shared_ptr<SurpherFunction>>(value_cast->get(Token("__sizeof__", {}, STRING, 0)))->call(interpreter, {});
    }
    else if (value.type() == typeid(std::string))
    {
        auto value_cast{std::any_cast<std::string>(value)};
        return static_cast<double>(value_cast.size());
    }

    return -1;
}

std::string NativeFunction::Sizeof::SurpherCallableToString()
{
    return {};
}

uint32_t NativeFunction::Floor::arity()
{
    return 1;
}

std::any NativeFunction::Floor::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];

    auto float_value = std::any_cast<double>(value);
    return std::floor(float_value);
}

std::string NativeFunction::Floor::SurpherCallableToString()
{
    return {};
}
