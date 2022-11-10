#include "Global.hpp"

uint32_t Sizeof::arity()
{
    return 1;
}

std::any Sizeof::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];

    if (value.type() == typeid(SurpherArrayPtr))
    {
        auto value_cast{std::any_cast<SurpherArrayPtr>(value)};
        return static_cast<long double>(value_cast->size());
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        auto value_cast{std::any_cast<std::shared_ptr<SurpherInstance>>(value)};
        return std::any_cast<std::shared_ptr<SurpherCallable>>(value_cast->get(Token("__sizeOf__", {}, STRING, 0)))->call(interpreter, {});
    }
    else if (value.type() == typeid(std::string))
    {
        auto value_cast{std::any_cast<std::string &>(value)};
        return static_cast<long double>(value_cast.size());
    }

    throw RuntimeError(paren, "Type not supported for \"sizeOf\".");
}

uint32_t SysCmd::arity()
{
    return 1;
}

std::any SysCmd::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(std::string))
        throw RuntimeError(paren, "System command must be a string.");

    return std::system((std::any_cast<std::string &>(value).c_str()));
}

uint32_t Equals::arity()
{
    return 2;
}

std::any Equals::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];

    if (op1.type() != op2.type())
    {
        return false;
    }

    if (op1.type() == typeid(long double))
    {
        return std::any_cast<long double &>(op1) == std::any_cast<long double &>(op2);
    }
    else if (op1.type() == typeid(bool))
    {
        return std::any_cast<bool>(op1) == std::any_cast<bool>(op2);
    }
    else if (op1.type() == typeid(std::string))
    {
        return std::any_cast<std::string &>(op1) == std::any_cast<std::string &>(op2);
    }
    else if (op1.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        auto op1_value{std::any_cast<std::shared_ptr<SurpherInstance>>(op1)};
        auto op2_value{std::any_cast<std::shared_ptr<SurpherInstance>>(op2)};
        return std::any_cast<std::shared_ptr<SurpherCallable>>(op1_value->get(Token("__equals__", {}, STRING, 0)))->call(interpreter, {op1, op2});
    }

    return false;
}