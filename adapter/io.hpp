#pragma once

#include "adapter/input.hpp"
#include "adapter/output.hpp"

namespace lib {


lib::input_adapter _input;
lib::output_adapter _print;


}

#define input lib::_input
#define print lib::_print
