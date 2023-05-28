#pragma once


#include <vector>
#include <algorithm>

#include "internal/dev_env.hpp"


namespace lib {


template<class T>
struct vector : std::vector<T> {
    using std::vector<T>::vector;

    inline auto& sort() noexcept(NO_EXCEPT) {
        std::sort(this->begin(), this->end());
        return *this;
    }

    template<class F>
    inline auto& sort(const F&& f) noexcept(NO_EXCEPT) {
        std::sort(this->begin(), this->end(), f);
        return *this;
    }
};


} // namespace lib
