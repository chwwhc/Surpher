#ifndef SURPHER_SURPHERCALLABLE_HPP
#define SURPHER_SURPHERCALLABLE_HPP

#include <vector>
#include <any>
#include <string>
#include <sstream>
#include <memory>

#include "Stmt.hpp"
#include "Environment.hpp"
#include "SurpherInstance.hpp"

class Interpreter;

struct SurpherCallable
{
    virtual uint32_t arity() = 0;
    virtual std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) = 0;
    virtual std::string SurpherCallableToString() = 0;
};

struct SurpherFunction : SurpherCallable
{
    const bool is_initializer;
    const bool is_partial;
    const bool is_sig;
    const std::shared_ptr<Environment> closure;
    const std::shared_ptr<Function> declaration;

    SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure, bool is_initializer, bool is_partial);
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;

    std::shared_ptr<SurpherCallable> bind(const std::shared_ptr<SurpherInstance> &instance);
};

struct SurpherClass : SurpherCallable, SurpherInstance
{
    const std::string name;

    std::unordered_map<std::string, std::shared_ptr<SurpherCallable>> instance_methods;

    std::unordered_map<std::string, std::shared_ptr<SurpherCallable>> class_methods;

    std::shared_ptr<SurpherCallable> superclass;

    SurpherClass(std::string name, std::unordered_map<std::string, std::shared_ptr<SurpherCallable>> instance_methods,
                 std::unordered_map<std::string, std::shared_ptr<SurpherCallable>> class_methods,
                 std::shared_ptr<SurpherCallable> superclass);

    uint32_t arity() override;

    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;

    std::string SurpherCallableToString() override;

    std::shared_ptr<SurpherCallable> findInstanceMethod(const std::string &methodName);

    std::shared_ptr<SurpherCallable> findClassMethod(const std::string &methodName);
};
#endif // SURPHER_SURPHERCALLABLE_HPP
