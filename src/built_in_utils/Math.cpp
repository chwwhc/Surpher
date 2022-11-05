#include <cmath>
#include <numeric>

#include "Math.hpp"

uint32_t Floor::arity(){
    return 1;
}

std::any Floor::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto value = arguments[0];
    if(value.type() != typeid(double)){
        throw RuntimeError(paren, "\"floor\" can only be applied to a numeric.");
    }

    return std::floor(std::any_cast<double>(value));
}

std::string Floor::SurpherCallableToString(){
    return {};
}

uint32_t Ceil::arity(){
    return 1;
}

std::any Ceil::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto value = arguments[0];
    if(value.type() != typeid(double)){
        throw RuntimeError(paren, "\"ceil\" can only be applied to a numeric.");
    }

    return std::ceil(std::any_cast<double>(value));
}

std::string Ceil::SurpherCallableToString(){
    return {};
}

uint32_t AbsoluteValue::arity(){
    return 1;
}

std::any AbsoluteValue::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto value = arguments[0];
    if(value.type() != typeid(double)){
        throw RuntimeError(paren, "\"abs\" can only be applied to a numeric.");
    }

    return std::floor(std::any_cast<double>(value));
}

std::string AbsoluteValue::SurpherCallableToString(){
    return {};
}

uint32_t Infinity::arity(){
    return 0;
}

std::any Infinity::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    return std::numeric_limits<double>::infinity();
}

std::string Infinity::SurpherCallableToString(){
    return {};
}

uint32_t Power::arity(){
    return 2;
}

std::any Power::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto num = arguments[0];
    auto pow = arguments[1];
    if(num.type() != typeid(double) || pow.type() != typeid(double)){
        throw RuntimeError(paren, "\"pow\" can only be applied to numerics.");
    }

    return std::pow(std::any_cast<double>(num), std::any_cast<double>(pow));
}

std::string Power::SurpherCallableToString(){
    return {};
}

uint32_t Sin::arity(){
    return 1;
}

std::any Sin::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto value = arguments[0];
    if(value.type() != typeid(double)){
        throw RuntimeError(paren, "\"sin\" can only be applied to a numeric.");
    }

    return std::sin(std::any_cast<double>(value));
}

std::string Sin::SurpherCallableToString(){
    return {};
}

uint32_t Cos::arity(){
    return 1;
}

std::any Cos::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto value = arguments[0];
    if(value.type() != typeid(double)){
        throw RuntimeError(paren, "\"cos\" can only be applied to a numeric.");
    }

    return std::cos(std::any_cast<double>(value));
}

std::string Cos::SurpherCallableToString(){
    return {};
}

uint32_t Tan::arity(){
    return 1;
}

std::any Tan::call(Interpreter &interpreter, const std::vector<std::any> &arguments){
    auto value = arguments[0];
    if(value.type() != typeid(double)){
        throw RuntimeError(paren, "\"tan\" can only be applied to a numeric.");
    }

    return std::tan(std::any_cast<double>(value));
}

std::string Tan::SurpherCallableToString(){
    return {};
}