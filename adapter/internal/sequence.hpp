#pragma once


#include <algorithm>

#include "internal/dev_env.hpp"


namespace lib {

namespace internal {


template<class Base>
struct extended_sequence : Base {
    using Base::Base;

    inline auto& sort() noexcept(NO_EXCEPT) {
        std::sort(std::begin(*this), std::end(*this));
        return *this;
    }

    template<class F>
    inline auto& sort(const F&& f) noexcept(NO_EXCEPT) {
        std::sort(std::begin(*this), std::end(*this), f);
        return *this;
    }

    inline auto& reverse() noexcept(NO_EXCEPT) {
        std::reverse(std::begin(*this), std::end(*this));
        return *this;
    }

    template<class T>
    inline auto binary_search(const T& v) noexcept(NO_EXCEPT) {
        return std::binary_search(std::begin(*this), std::end(*this), v);
    }

    template<class T>
    inline auto lower_bound(const T& v) noexcept(NO_EXCEPT) {
        return std::lower_bound(std::begin(*this), std::end(*this), v);
    }

    template<class T>
    inline auto upper_bound(const T& v) noexcept(NO_EXCEPT) {
        return std::upper_bound(std::begin(*this), std::end(*this), v);
    }
};


} // namespace internal

} // namespace lib
