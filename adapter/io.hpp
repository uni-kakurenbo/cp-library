#pragma once


#include "adapter/internal/input.hpp"
#include "adapter/internal/output.hpp"


namespace lib {


lib::input_adapter _input;
lib::output_adapter _print;


}


#define input lib::_input
#define print lib::_print
