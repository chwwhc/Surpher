#include <cmath>
#include <complex>
#include <numeric>

#include "Math.hpp"

uint32_t Floor::arity()
{
    return 1;
}

std::any Floor::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"floor\" can only be applied to a numeric.");
    }

    return std::floor(std::any_cast<long double>(value));
}

uint32_t Ceil::arity()
{
    return 1;
}

std::any Ceil::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"ceil\" can only be applied to a numeric.");
    }

    return std::ceil(std::any_cast<long double>(value));
}

uint32_t AbsoluteValue::arity()
{
    return 1;
}

std::any AbsoluteValue::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(long double) && value.type() != typeid(std::complex<long double>))
    {
        throw RuntimeError(paren, "\"abs\" can only be applied to a numeric.");
    }

    if (value.type() == typeid(long double))
    {
        return std::abs(std::any_cast<long double>(value));
    }

    return std::abs(std::any_cast<std::complex<long double>>(value));
}

uint32_t Infinity::arity()
{
    return 0;
}

std::any Infinity::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    return std::numeric_limits<long double>::infinity();
}

uint32_t Power::arity()
{
    return 2;
}

std::any Power::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto num = arguments[0];
    auto pow = arguments[1];
    if (num.type() != typeid(long double) || pow.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"pow\" can only be applied to numerics.");
    }

    return std::pow(std::any_cast<long double>(num), std::any_cast<long double>(pow));
}

uint32_t Sin::arity()
{
    return 1;
}

std::any Sin::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"sin\" can only be applied to a numeric.");
    }

    return std::sin(std::any_cast<long double>(value));
}

uint32_t Cos::arity()
{
    return 1;
}

std::any Cos::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"cos\" can only be applied to a numeric.");
    }

    return std::cos(std::any_cast<long double>(value));
}

uint32_t Tan::arity()
{
    return 1;
}

std::any Tan::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"tan\" can only be applied to a numeric.");
    }

    return std::tan(std::any_cast<long double>(value));
}

uint32_t ComplexNumber::arity()
{
    return 2;
}

std::any ComplexNumber::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto real = arguments[0];
    auto img = arguments[1];
    if (real.type() != typeid(long double) || img.type() != typeid(long double))
    {
        throw RuntimeError(paren, "\"complexNum\" can only be applied to numerics.");
    }

    return std::complex(std::any_cast<long double>(real), std::any_cast<long double>(img));
}

uint32_t ComplexAdd::arity()
{
    return 2;
}

std::any ComplexAdd::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<long double>) || op2.type() != typeid(std::complex<long double>))
    {
        throw RuntimeError(paren, "\"complexAdd\" can only be applied to complex numbers.");
    }

    return std::any_cast<std::complex<long double>>(op1) + std::any_cast<std::complex<long double>>(op2);
}

uint32_t ComplexSub::arity()
{
    return 2;
}

std::any ComplexSub::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<long double>) || op2.type() != typeid(std::complex<long double>))
    {
        throw RuntimeError(paren, "\"complexSub\" can only be applied to complex numbers.");
    }

    return std::any_cast<std::complex<long double>>(op1) - std::any_cast<std::complex<long double>>(op2);
}

uint32_t ComplexMul::arity()
{
    return 2;
}

std::any ComplexMul::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<long double>) || op2.type() != typeid(std::complex<long double>))
    {
        throw RuntimeError(paren, "\"complexMul\" can only be applied to complex numbers.");
    }

    return std::any_cast<std::complex<long double>>(op1) * std::any_cast<std::complex<long double>>(op2);
}

uint32_t ComplexDiv::arity()
{
    return 2;
}

std::any ComplexDiv::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<long double>) || op2.type() != typeid(std::complex<long double>))
    {
        throw RuntimeError(paren, "\"complexDiv\" can only be applied to complex numbers.");
    }
    else if (std::any_cast<std::complex<long double>>(op2) == std::complex<long double>(0, 0))
    {
        throw RuntimeError(paren, "\"complexDiv\"'s denominator cannot be 0.");
    }

    return std::any_cast<std::complex<long double>>(op1) / std::any_cast<std::complex<long double>>(op2);
}