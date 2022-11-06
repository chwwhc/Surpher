#include "NativeFunction.hpp"

struct Ascii : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct ToNumber : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};

struct ToString : NativeFunction
{
    uint32_t arity() override;
    std::any call(Interpreter &interpreter, const std::vector<std::any> &arguments) override;
};