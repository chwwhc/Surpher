#include "Global.hpp"

uint32_t Sizeof::arity()
{
    return 1;
}

std::any Sizeof::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(std::shared_ptr<SurpherArray>) && value.type() != typeid(std::shared_ptr<SurpherInstance>) && value.type() != typeid(std::string))
        throw RuntimeError(paren, "Type not supported for \"sizeof\".");

    if (value.type() == typeid(std::shared_ptr<SurpherArray>))
    {
        auto value_cast{std::any_cast<std::shared_ptr<SurpherArray>>(value)};
        return static_cast<double>(value_cast->size());
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        auto value_cast{std::any_cast<std::shared_ptr<SurpherInstance>>(value)};
        return std::any_cast<std::shared_ptr<SurpherFunction>>(value_cast->get(Token("__sizeof__", {}, STRING, 0)))->call(interpreter, {});
    }
    else if (value.type() == typeid(std::string))
    {
        auto value_cast{std::any_cast<std::string>(value)};
        return static_cast<double>(value_cast.size());
    }

    return {};
}

std::string Sizeof::SurpherCallableToString()
{
    return {};
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

    std::system((std::any_cast<std::string>(value).c_str()));

    return {};
}

std::string SysCmd::SurpherCallableToString()
{
    return {};
}