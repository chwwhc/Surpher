#pragma once

#include <fstream>
#include <sstream>
#include "NativeFunction.hpp"

struct FileOpen : NativeFunction
{
    FileOpen(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct Write : NativeFunction
{
    Write(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ReadAll : NativeFunction
{
    ReadAll(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct ReadSome : NativeFunction
{
    ReadSome(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};

struct FileClose : NativeFunction
{
    FileClose(Token paren) : NativeFunction(paren) {}

    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
    std::string SurpherCallableToString() override;
};