#pragma once


#include <string>
#include <algorithm>

#include "adapter/internal/container_extender.hpp"


namespace lib {


using string = internal::extended_container<std::string>;


} // namespace lib
