#pragma once


#include <string>
#include <algorithm>

#include "adaptor/internal/advanced_container.hpp"


namespace uni {


using string = internal::advanced_container<std::string>;


} // namespace uni


namespace std {


template<>
struct hash<uni::string> {
    inline auto operator()(const uni::string& key) const noexcept(NO_EXCEPT) {
        return std::hash<std::string>{}(static_cast<std::string>(key));
    }
};



}
