#pragma once

#include <valarray>
#include <algorithm>
#include <type_traits>

#include "internal/types.hpp"

namespace lib {

template<class T> struct valarray : std::valarray<T> {
    using std::valarray<T>::valarray;

    valarray(const internal::size_t length, const T &&val = T{}) : std::valarray<T>(std::forward<const T>(val), length) {}

    inline void reserve(const internal::size_t) {}

    template<class I, std::enable_if_t<std::is_same_v<T,typename std::iterator_traits<I>::value_type>>* = nullptr>
    inline void assign(const I first, const I last) {
        this->resize(std::distance(first, last));
        std::copy(first, last, begin(*this));
    }

    inline void assign(const internal::size_t length, const T &&val = T{}) {
        this->std::valarray<T>::resize(length, std::forward<const T>(val));
    }

    inline void resize(const internal::size_t length, const T &&val = T{}) {
        std::valarray<T> temp = *this;
        this->assign(length, std::forward<const T>(val));
        std::move(begin(temp), std::min(std::end(temp), std::next(std::begin(temp), length)), begin(*this));
    }
};

} // namespace lib
