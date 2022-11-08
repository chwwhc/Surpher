#pragma once

#include "NativeFunction.hpp"

struct Thread : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct Join : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct Mutex : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct Lock : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct Unlock : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};