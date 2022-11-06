#include "Utils.hpp"

std::shared_ptr<SurpherNamespace> IO()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("fileOpen", std::shared_ptr<NativeFunction>(new FileOpen()), true);
    new_environment->define("fileWrite", std::shared_ptr<NativeFunction>(new Write()), true);
    new_environment->define("fileReadAll", std::shared_ptr<NativeFunction>(new ReadAll()), true);
    new_environment->define("fileReadSome", std::shared_ptr<NativeFunction>(new ReadSome()), true);
    new_environment->define("fileClose", std::shared_ptr<NativeFunction>(new FileClose()), true);
    new_environment->define("input", std::shared_ptr<NativeFunction>(new Input()), true);
    return std::make_shared<SurpherNamespace>("IO", new_environment);
}

std::shared_ptr<SurpherNamespace> String()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("ascii", std::shared_ptr<NativeFunction>(new Ascii()), true);
    new_environment->define("toNumber", std::shared_ptr<NativeFunction>(new ToNumber()), true);
    new_environment->define("toString", std::shared_ptr<NativeFunction>(new ToString()), true);
    return std::make_shared<SurpherNamespace>("String", new_environment);
}

std::shared_ptr<SurpherNamespace> Math()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("complexNum", std::shared_ptr<NativeFunction>(new ComplexNumber()), true);
    new_environment->define("complexAdd", std::shared_ptr<NativeFunction>(new ComplexAdd()), true);
    new_environment->define("complexSub", std::shared_ptr<NativeFunction>(new ComplexSub()), true);
    new_environment->define("complexMul", std::shared_ptr<NativeFunction>(new ComplexMul()), true);
    new_environment->define("complexDiv", std::shared_ptr<NativeFunction>(new ComplexDiv()), true);
    new_environment->define("floor", std::shared_ptr<NativeFunction>(new Floor()), true);
    new_environment->define("ceil", std::shared_ptr<NativeFunction>(new Ceil()), true);
    new_environment->define("abs", std::shared_ptr<NativeFunction>(new AbsoluteValue()), true);
    new_environment->define("inf", std::shared_ptr<NativeFunction>(new Infinity()), true);
    new_environment->define("pow", std::shared_ptr<NativeFunction>(new Power()), true);
    new_environment->define("sin", std::shared_ptr<NativeFunction>(new Sin()), true);
    new_environment->define("cos", std::shared_ptr<NativeFunction>(new Cos()), true);
    new_environment->define("tan", std::shared_ptr<NativeFunction>(new Tan()), true);
    return std::make_shared<SurpherNamespace>("Math", new_environment);
}

void glodbalFunctionSetup(Environment &environment)
{
    environment.define("sizeOf", std::shared_ptr<NativeFunction>(new Sizeof()), true);
    environment.define("systemCall", std::shared_ptr<NativeFunction>(new SysCmd()), true);
    environment.define("equals", std::shared_ptr<NativeFunction>(new Equals()), true);
}