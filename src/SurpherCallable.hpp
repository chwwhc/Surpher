#ifndef SURPHER_SURPHERCALLABLE_HPP
#define SURPHER_SURPHERCALLABLE_HPP

#include <vector>
#include <utility>
#include <any>
#include <chrono>
#include <string>
#include <memory>
#include "Stmt.hpp"
#include "Environment.hpp"
#include "SurpherInstance.hpp"

class Interpreter;

struct SurpherCallable{
    virtual uint32_t arity() = 0;
    virtual std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) = 0;
    virtual std::string SurpherCallableToString() = 0;
};

struct SurpherFunction : SurpherCallable{
    const bool is_initializer;
    const bool is_partial;
    const std::shared_ptr<Environment> closure;
    const std::shared_ptr<Function> declaration;

    SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure, bool is_initializer, bool is_partial);
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;

    std::shared_ptr<SurpherFunction> bind(const std::shared_ptr<SurpherInstance> &instance);
};

struct SurpherClass : SurpherCallable, SurpherInstance {
    const std::string name;

    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> instance_methods;

    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> class_methods;

    const std::shared_ptr<SurpherClass> superclass;

    SurpherClass(std::string name, std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> instance_methods,
                 std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> class_methods,
                 std::shared_ptr<SurpherClass> superclass);

    uint32_t arity() override;

    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;

    std::string SurpherCallableToString() override;

    std::shared_ptr<SurpherFunction> findInstanceMethod(const std::string& methodName);

    std::shared_ptr<SurpherFunction> findClassMethod(const std::string& methodName);
};

struct Clock : public SurpherCallable {
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

#endif //SURPHER_SURPHERCALLABLE_HPP
