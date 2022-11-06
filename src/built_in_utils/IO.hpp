#pragma once

#include <fstream>
#include <sstream>
#include "NativeFunction.hpp"

struct Input : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct FileOpen : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct Write : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct ReadAll : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct ReadSome : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct FileClose : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};