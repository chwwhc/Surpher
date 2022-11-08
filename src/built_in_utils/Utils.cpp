#include "Utils.hpp"

/*
std::shared_ptr<SurpherNamespace> Concurrency()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("thread", std::shared_ptr<SurpherCallable>(new Thread()), true);
    new_environment->define("join", std::shared_ptr<SurpherCallable>(new Join()), true);
    new_environment->define("mutex", std::shared_ptr<SurpherCallable>(new Mutex()), true);
    new_environment->define("lock", std::shared_ptr<SurpherCallable>(new Lock()), true);
    new_environment->define("unlock", std::shared_ptr<SurpherCallable>(new Unlock()), true);
    return std::make_shared<SurpherNamespace>("Concurrency", new_environment);
}
*/

std::shared_ptr<SurpherNamespace> Chrono()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("clock", std::shared_ptr<SurpherCallable>(new Clock()), true);
    return std::make_shared<SurpherNamespace>("Chrono", new_environment);
}

std::shared_ptr<SurpherNamespace> IO()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("fileOpen", std::shared_ptr<SurpherCallable>(new FileOpen()), true);
    new_environment->define("fileWrite", std::shared_ptr<SurpherCallable>(new Write()), true);
    new_environment->define("fileReadAll", std::shared_ptr<SurpherCallable>(new ReadAll()), true);
    new_environment->define("fileReadSome", std::shared_ptr<SurpherCallable>(new ReadSome()), true);
    new_environment->define("fileClose", std::shared_ptr<SurpherCallable>(new FileClose()), true);
    new_environment->define("input", std::shared_ptr<SurpherCallable>(new Input()), true);
    return std::make_shared<SurpherNamespace>("IO", new_environment);
}

std::shared_ptr<SurpherNamespace> String()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("ascii", std::shared_ptr<SurpherCallable>(new Ascii()), true);
    new_environment->define("toNumber", std::shared_ptr<SurpherCallable>(new ToNumber()), true);
    new_environment->define("toString", std::shared_ptr<SurpherCallable>(new ToString()), true);
    return std::make_shared<SurpherNamespace>("String", new_environment);
}

std::shared_ptr<SurpherNamespace> Math()
{
    std::shared_ptr<Environment> new_environment = std::make_shared<Environment>();
    new_environment->define("complexNum", std::shared_ptr<SurpherCallable>(new ComplexNumber()), true);
    new_environment->define("complexAdd", std::shared_ptr<SurpherCallable>(new ComplexAdd()), true);
    new_environment->define("complexSub", std::shared_ptr<SurpherCallable>(new ComplexSub()), true);
    new_environment->define("complexMul", std::shared_ptr<SurpherCallable>(new ComplexMul()), true);
    new_environment->define("complexDiv", std::shared_ptr<SurpherCallable>(new ComplexDiv()), true);
    new_environment->define("floor", std::shared_ptr<SurpherCallable>(new Floor()), true);
    new_environment->define("ceil", std::shared_ptr<SurpherCallable>(new Ceil()), true);
    new_environment->define("abs", std::shared_ptr<SurpherCallable>(new AbsoluteValue()), true);
    new_environment->define("inf", std::shared_ptr<SurpherCallable>(new Infinity()), true);
    new_environment->define("pow", std::shared_ptr<SurpherCallable>(new Power()), true);
    new_environment->define("sin", std::shared_ptr<SurpherCallable>(new Sin()), true);
    new_environment->define("cos", std::shared_ptr<SurpherCallable>(new Cos()), true);
    new_environment->define("tan", std::shared_ptr<SurpherCallable>(new Tan()), true);
    return std::make_shared<SurpherNamespace>("Math", new_environment);
}

void glodbalFunctionSetup(Environment &environment)
{
    environment.define("sizeOf", std::shared_ptr<SurpherCallable>(new Sizeof()), true);
    environment.define("systemCall", std::shared_ptr<SurpherCallable>(new SysCmd()), true);
    environment.define("equals", std::shared_ptr<SurpherCallable>(new Equals()), true);
}