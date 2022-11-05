#pragma once

#include "../SurpherCallable.hpp"
#include "../SurpherNamespace.hpp"
#include "../Error.hpp"

struct NativeFunction : SurpherCallable
{
    Token paren;

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