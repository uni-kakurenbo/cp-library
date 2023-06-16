#pragma once

#include "adapter/input.hpp"
#include "adapter/output.hpp"

lib::input_adapter _input;
lib::output_adapter _print;

#define input _input
#define print _print
