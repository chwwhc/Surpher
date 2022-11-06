#pragma once

#include "../SurpherCallable.hpp"
#include "../SurpherNamespace.hpp"
#include "../Error.hpp"

using SurpherArray = std::vector<std::any>;
using SurpherArrayPtr = std::shared_ptr<std::vector<std::any>>;

struct NativeFunction : SurpherCallable
{
    Token paren;

    NativeFunction() = default;
    NativeFunction(Token paren) : paren(std::move(paren)) {}

    virtual uint32_t arity() override
    {
        throw RuntimeError(paren, "Unimplemented native function.");
    }
    virtual std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override
    {
        throw RuntimeError(paren, "Unimplemented native function.");
    }
    virtual std::string SurpherCallableToString() override
    {
        throw RuntimeError(paren, "Unimplemented native function.");
    }
};