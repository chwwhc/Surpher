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

struct SurpherCallable{
    virtual uint32_t arity() = 0;
    virtual std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) = 0;
    virtual std::string SurpherCallableToString() = 0;
};

class SurpherFunction : public SurpherCallable{
    const std::shared_ptr<Function> declaration;
    const std::shared_ptr<Environment> closure;
public:
    SurpherFunction(std::shared_ptr<Function> declaration, std::shared_ptr<Environment> closure);
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Clock : public SurpherCallable {
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

#endif //SURPHER_SURPHERCALLABLE_HPP
