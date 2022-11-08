#pragma once

#include "IO.hpp"
#include "Global.hpp"
#include "Concurrency.hpp"
#include "Chrono.hpp"
#include "String.hpp"
#include "Math.hpp"
#include "../Environment.hpp"

std::shared_ptr<SurpherNamespace> IO();

std::shared_ptr<SurpherNamespace> Math();

std::shared_ptr<SurpherNamespace> Chrono();

//std::shared_ptr<SurpherNamespace> Concurrency();

std::shared_ptr<SurpherNamespace> String();

void glodbalFunctionSetup(Environment& environment);