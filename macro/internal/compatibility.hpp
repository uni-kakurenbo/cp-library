#pragma once


#include <string>
#include "adaptor/string.hpp"


#if __GNUG__ >= 13
#   define UNI_CONSTEXPR_STD_STRING constexpr std::string
#   define UNI_CONSTEXPR_STRING constexpr uni::string
#else
#   define UNI_CONSTEXPR_STD_STRING const std::string
#   define UNI_CONSTEXPR_STRING const uni::string
#endif
