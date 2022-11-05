#pragma once

#include "IO.hpp"
#include "Global.hpp"
#include "Math.hpp"
#include "../Environment.hpp"

std::shared_ptr<SurpherNamespace> IO();

std::shared_ptr<SurpherNamespace> Math();

void glodbalFunctionSetup(Environment& environment);