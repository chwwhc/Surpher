#include <chrono>

#include "Chrono.hpp"

uint32_t Clock::arity()
{
    return 0;
}

std::any Clock::call(Interpreter &interpreter, const std::vector<std::any> &arguments)
{
    return std::chrono::duration<long double>{std::chrono::system_clock::now().time_since_epoch()}.count();
}