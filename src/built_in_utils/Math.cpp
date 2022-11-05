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
    if (value.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"floor\" can only be applied to a numeric.");
    }

    return std::floor(std::any_cast<double>(value));
}

std::string Floor::SurpherCallableToString()
{
    return {};
}

uint32_t Ceil::arity()
{
    return 1;
}

std::any Ceil::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"ceil\" can only be applied to a numeric.");
    }

    return std::ceil(std::any_cast<double>(value));
}

std::string Ceil::SurpherCallableToString()
{
    return {};
}

uint32_t AbsoluteValue::arity()
{
    return 1;
}

std::any AbsoluteValue::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(double) && value.type() != typeid(std::complex<double>))
    {
        throw RuntimeError(paren, "\"abs\" can only be applied to a numeric.");
    }

    if (value.type() == typeid(double))
    {
        return std::abs(std::any_cast<double>(value));
    }

    return std::abs(std::any_cast<std::complex<double>>(value));
}

std::string AbsoluteValue::SurpherCallableToString()
{
    return {};
}

uint32_t Infinity::arity()
{
    return 0;
}

std::any Infinity::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    return std::numeric_limits<double>::infinity();
}

std::string Infinity::SurpherCallableToString()
{
    return {};
}

uint32_t Power::arity()
{
    return 2;
}

std::any Power::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto num = arguments[0];
    auto pow = arguments[1];
    if (num.type() != typeid(double) || pow.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"pow\" can only be applied to numerics.");
    }

    return std::pow(std::any_cast<double>(num), std::any_cast<double>(pow));
}

std::string Power::SurpherCallableToString()
{
    return {};
}

uint32_t Sin::arity()
{
    return 1;
}

std::any Sin::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"sin\" can only be applied to a numeric.");
    }

    return std::sin(std::any_cast<double>(value));
}

std::string Sin::SurpherCallableToString()
{
    return {};
}

uint32_t Cos::arity()
{
    return 1;
}

std::any Cos::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"cos\" can only be applied to a numeric.");
    }

    return std::cos(std::any_cast<double>(value));
}

std::string Cos::SurpherCallableToString()
{
    return {};
}

uint32_t Tan::arity()
{
    return 1;
}

std::any Tan::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto value = arguments[0];
    if (value.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"tan\" can only be applied to a numeric.");
    }

    return std::tan(std::any_cast<double>(value));
}

std::string Tan::SurpherCallableToString()
{
    return {};
}

uint32_t ComplexNumber::arity()
{
    return 2;
}

std::any ComplexNumber::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto real = arguments[0];
    auto img = arguments[1];
    if (real.type() != typeid(double) || img.type() != typeid(double))
    {
        throw RuntimeError(paren, "\"complexNum\" can only be applied to numerics.");
    }

    return std::complex(std::any_cast<double>(real), std::any_cast<double>(img));
}

std::string ComplexNumber::SurpherCallableToString()
{
    return {};
}

uint32_t ComplexAdd::arity()
{
    return 2;
}

std::any ComplexAdd::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<double>) || op2.type() != typeid(std::complex<double>))
    {
        throw RuntimeError(paren, "\"complexAdd\" can only be applied to complex numbers.");
    }

    return std::any_cast<std::complex<double>>(op1) + std::any_cast<std::complex<double>>(op2);
}

std::string ComplexAdd::SurpherCallableToString()
{
    return {};
}

uint32_t ComplexSub::arity()
{
    return 2;
}

std::any ComplexSub::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<double>) || op2.type() != typeid(std::complex<double>))
    {
        throw RuntimeError(paren, "\"complexSub\" can only be applied to complex numbers.");
    }

    return std::any_cast<std::complex<double>>(op1) - std::any_cast<std::complex<double>>(op2);
}

std::string ComplexSub::SurpherCallableToString()
{
    return {};
}

uint32_t ComplexMul::arity()
{
    return 2;
}

std::any ComplexMul::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<double>) || op2.type() != typeid(std::complex<double>))
    {
        throw RuntimeError(paren, "\"complexMul\" can only be applied to complex numbers.");
    }

    return std::any_cast<std::complex<double>>(op1) * std::any_cast<std::complex<double>>(op2);
}

std::string ComplexMul::SurpherCallableToString()
{
    return {};
}

uint32_t ComplexDiv::arity()
{
    return 2;
}

std::any ComplexDiv::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    auto op1 = arguments[0];
    auto op2 = arguments[1];
    if (op1.type() != typeid(std::complex<double>) || op2.type() != typeid(std::complex<double>))
    {
        throw RuntimeError(paren, "\"complexDiv\" can only be applied to complex numbers.");
    }
    else if (std::any_cast<std::complex<double>>(op2) == std::complex<double>(0, 0))
    {
        throw RuntimeError(paren, "\"complexDiv\"'s denominator cannot be 0.");
    }

    return std::any_cast<std::complex<double>>(op1) / std::any_cast<std::complex<double>>(op2);
}

std::string ComplexDiv::SurpherCallableToString()
{
    return {};
}