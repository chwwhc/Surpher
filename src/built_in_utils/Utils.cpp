#include "Utils.hpp"

Token dummy;

std::shared_ptr<SurpherNamespace> IO()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("fileOpen", std::shared_ptr<NativeFunction>(new FileOpen(dummy)), true);
    new_environment->define("fileWrite", std::shared_ptr<NativeFunction>(new Write(dummy)), true);
    new_environment->define("fileReadAll", std::shared_ptr<NativeFunction>(new ReadAll(dummy)), true);
    new_environment->define("fileReadSome", std::shared_ptr<NativeFunction>(new ReadSome(dummy)), true);
    new_environment->define("fileClose", std::shared_ptr<NativeFunction>(new FileClose(dummy)), true);
    return std::make_shared<SurpherNamespace>("IO", new_environment);
}

std::shared_ptr<SurpherNamespace> Math()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("floor", std::shared_ptr<NativeFunction>(new Floor(dummy)), true);
    new_environment->define("ceil", std::shared_ptr<NativeFunction>(new Ceil(dummy)), true);
    new_environment->define("abs", std::shared_ptr<NativeFunction>(new AbsoluteValue(dummy)), true);
    new_environment->define("inf", std::shared_ptr<NativeFunction>(new Infinity(dummy)), true);
    new_environment->define("pow", std::shared_ptr<NativeFunction>(new Power(dummy)), true);
    new_environment->define("sin", std::shared_ptr<NativeFunction>(new Sin(dummy)), true);
    new_environment->define("cos", std::shared_ptr<NativeFunction>(new Cos(dummy)), true);
    new_environment->define("tan", std::shared_ptr<NativeFunction>(new Tan(dummy)), true);
    return std::make_shared<SurpherNamespace>("Math", new_environment);
}

void glodbalFunctionSetup(Environment &environment)
{
    environment.define("sizeof", std::shared_ptr<NativeFunction>(new Sizeof(dummy)), true);
    environment.define("systemCall", std::shared_ptr<NativeFunction>(new SysCmd(dummy)), true);
}