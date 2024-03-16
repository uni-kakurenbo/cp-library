#pragma once


#include "adaptor/internal/input.hpp"
#include "adaptor/internal/output.hpp"


namespace lib {


lib::input_adaptor _input;
lib::output_adaptor _print;


}


#define input lib::_input
#define print lib::_print
