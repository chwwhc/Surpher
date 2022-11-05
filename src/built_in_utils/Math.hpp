#pragma once

#include "NativeFunction.hpp"

struct Floor : NativeFunction
{
    Floor(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Ceil : NativeFunction
{
    Ceil(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct AbsoluteValue : NativeFunction
{
    AbsoluteValue(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Infinity : NativeFunction
{
    Infinity(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Power : NativeFunction
{
    Power(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Sin : NativeFunction
{
    Sin(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Cos : NativeFunction
{
    Cos(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Tan : NativeFunction
{
    Tan(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ComplexNumber : NativeFunction
{
    ComplexNumber(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ComplexAdd : NativeFunction
{
    ComplexAdd(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ComplexSub : NativeFunction
{
    ComplexSub(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ComplexMul : NativeFunction
{
    ComplexMul(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ComplexDiv : NativeFunction
{
    ComplexDiv(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};