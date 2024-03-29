#include <utility>
#include <fstream>
#include <execution>
#include <cmath>

#include "SurpherCallable.hpp"
#include "Error.hpp"
#include "Interpreter.hpp"

using namespace std::string_literals;

SurpherFunction::SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure,
                                 bool is_initializer, bool is_partial) : is_sig(declaration->is_sig), declaration(std::move(declaration)),
                                                                         closure(std::move(closure)), is_initializer(is_initializer),
                                                                         is_partial(is_partial)
{
}

std::any SurpherFunction::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto environment{is_partial ? closure : std::make_shared<Environment>(closure)};

#pragma omp parallel for
    {
        for (size_t i = 0; i < declaration->params.size(); i++)
        {
            environment->define(declaration->params[i], arguments[i], false);
        }
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

std::shared_ptr<SurpherCallable> SurpherFunction::bind(const std::shared_ptr<SurpherInstance> &instance)
{
    auto environment = std::make_shared<Environment>(closure);
    environment->define("this", instance, true);
    return std::make_shared<SurpherFunction>(declaration, std::move(environment), is_initializer, is_partial);
}

SurpherClass::SurpherClass(std::string name, std::unordered_map<std::string, std::shared_ptr<SurpherCallable>> instance_methods,
                           std::unordered_map<std::string, std::shared_ptr<SurpherCallable>> class_methods,
                           std::shared_ptr<SurpherCallable> superclass) : SurpherInstance(nullptr), name(std::move(name)), instance_methods(std::move(instance_methods)),
                                                                          class_methods(std::move(class_methods)),
                                                                          superclass(std::move(superclass))
{
}

std::any SurpherClass::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto instance(std::make_shared<SurpherInstance>(std::make_shared<SurpherClass>(*this)));
    auto initializer(findInstanceMethod("init"));

    if (initializer)
        dynamic_cast<SurpherFunction *>(initializer.get())->bind(instance)->call(interpreter, arguments);

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

std::shared_ptr<SurpherCallable> SurpherClass::findInstanceMethod(const std::string &methodName)
{
    if (instance_methods.find(methodName) != instance_methods.end())
        return instance_methods[methodName];

    if (superclass != nullptr)
        return std::dynamic_pointer_cast<SurpherClass>(superclass)->findInstanceMethod(methodName);

    return {};
}

std::shared_ptr<SurpherCallable> SurpherClass::findClassMethod(const std::string &methodName)
{
    if (class_methods.find(methodName) != class_methods.end())
        return class_methods[methodName];

    if (superclass != nullptr)
        return std::dynamic_pointer_cast<SurpherClass>(superclass)->findClassMethod(methodName);

    return {};
}