#include <cmath>

#include "String.hpp"

uint32_t Ascii::arity()
{
    return 1;
}

std::any Ascii::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto any_value = arguments[0];
    if (any_value.type() == typeid(std::string))
    {
        auto value = std::any_cast<std::string>(any_value);
        if (value.size() == 1)
        {
            return static_cast<long double>(value[0]);
        }
    }

    throw RuntimeError(paren, "Invalid usage of \"ascii\". Usage: ascii(<string of size 1>).");
}

uint32_t ToNumber::arity()
{
    return 1;
}

std::any ToNumber::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto any_value = arguments[0];
    if (any_value.type() == typeid(std::string))
    {
        auto value = std::any_cast<std::string>(any_value);
        long double ret;
        try
        {
            ret = std::stold(value);
            return ret;
        }
        catch (const std::exception &e)
        {
            throw RuntimeError(paren, "Invalid usage of \"toNumber\". Usage: toNumber(<string encoded number>).");
        }
    }

    throw RuntimeError(paren, "Invalid usage of \"toNumber\". Usage: toNumber(<string encoded number>).");
}


uint32_t ToString::arity()
{
    return 1;
}

std::any ToString::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];

    if (value.type() == typeid(nullptr))
    {
        return std::string("nil");
    }
    else if (value.type() == typeid(long double))
    {
        auto double_val(std::any_cast<long double>(value));
        std::string num_str(std::to_string(double_val));
        if (floor(double_val) == double_val)
        {
            uint32_t point_index = 0;
            while (point_index < num_str.size() && num_str[point_index] != '.')
            {
                point_index++;
            }
            return num_str.substr(0, point_index);
        }
        return num_str;
    }
    else if (value.type() == typeid(std::string))
    {
        return std::any_cast<std::string>(value);
    }
    else if (value.type() == typeid(bool))
    {
        return std::any_cast<bool>(value) ? "true" : "false";
    }
    else if (value.type() == typeid(std::shared_ptr<SurpherInstance>))
    {
        auto value_cast{std::any_cast<std::shared_ptr<SurpherInstance>>(value)};
        return std::any_cast<std::shared_ptr<SurpherFunction>>(value_cast->get(Token("__toString__", {}, STRING, 0)))->call(interpreter, {});
    }
    else if (value.type() == typeid(SurpherArrayPtr))
    {
        auto expr_vector{std::any_cast<SurpherArrayPtr>(value)};
        if (expr_vector->empty())
        {
            return std::string("[]");
        }

        std::ostringstream str_builder;
        str_builder << "[";
        for (const auto &expr : *expr_vector)
            str_builder << std::any_cast<std::string>(call(interpreter, {expr})) << ", ";
        auto expr_vector_str{str_builder.str()};

        expr_vector_str.pop_back();
        expr_vector_str.pop_back();

        expr_vector_str.push_back(']');

        return expr_vector_str;
    }
    else
    {
        throw RuntimeError(paren, "Type not supported for \"toString\".");
    }
}