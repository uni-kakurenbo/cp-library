#pragma once


#include "adaptor/internal/input.hpp"
#include "adaptor/internal/output.hpp"


namespace uni {


uni::input_adaptor _input;
uni::output_adaptor _print;


}


#define input uni::_input
#define print uni::_print
