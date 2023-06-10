#pragma once

#include "adapter/input.hpp"
#include "adapter/output.hpp"

input_adapter _input;
output_adapter _print;

#define input _input
#define print _print
