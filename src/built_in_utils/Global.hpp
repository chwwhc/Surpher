#pragma once

#include "NativeFunction.hpp"

using SurpherArray = std::vector<std::any>;
using SurpherArrayPtr = std::shared_ptr<std::vector<std::any>>;

struct Sizeof : NativeFunction
{
    Sizeof(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct SysCmd : NativeFunction
{
    SysCmd(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};
