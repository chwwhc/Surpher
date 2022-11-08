#pragma once

#include "NativeFunction.hpp"

struct Clock : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};