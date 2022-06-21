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

class Interpreter;
class SurpherInstance;

struct SurpherCallable{
    virtual uint32_t arity() = 0;
    virtual std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) = 0;
    virtual std::string SurpherCallableToString() = 0;
};

class SurpherFunction : public SurpherCallable{
    const std::shared_ptr<Function> declaration;
    const std::shared_ptr<Environment> closure;
    const bool is_initializer;
public:
    SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure, bool is_initializer);
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
    std::shared_ptr<SurpherFunction> bind(const std::shared_ptr<SurpherInstance> &instance);
};

struct SurpherClass : SurpherCallable, public std::enable_shared_from_this<SurpherClass>{
    const std::string name;

    std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> methods;

    SurpherClass(std::string name, std::unordered_map<std::string, std::shared_ptr<SurpherFunction>> methods);

    uint32_t arity() override;

    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;

    std::string SurpherCallableToString() override;

    std::shared_ptr<SurpherFunction> findMethod(const std::string& methodName);
};

struct Clock : public SurpherCallable {
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

#endif //SURPHER_SURPHERCALLABLE_HPP
