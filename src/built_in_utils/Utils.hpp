#pragma once

#include "IO.hpp"
#include "Global.hpp"
#include "String.hpp"
#include "Math.hpp"
#include "../Environment.hpp"

std::shared_ptr<SurpherNamespace> IO();

std::shared_ptr<SurpherNamespace> Math();

std::shared_ptr<SurpherNamespace> String();

void glodbalFunctionSetup(Environment& environment);