#pragma once

#include <valarray>
#include <algorithm>
#include <type_traits>

#include "internal/types.hpp"

namespace Lib {

template<class T> struct ValArray : std::valarray<T> {
    using valarray<T>::valarray;

    ValArray(const Internal::Size length, const T &&val = T{}) : std::valarray<T>(std::forward<const T>(val), length) {}

    inline void reserve(const Internal::Size) {}

    template<class I, std::enable_if_t<std::is_same_v<T,typename std::iterator_traits<I>::value_type>,std::nullptr_t> = nullptr>
    inline void assign(const I first, const I last) {
        this->resize(std::distance(first, last));
        std::copy(first, last, begin(*this));
    }

    inline void assign(const Internal::Size length, const T &&val = T{}) {
        this->std::valarray<T>::resize(length, std::forward<const T>(val));
    }

    inline void resize(const Internal::Size length, const T &&val = T{}) {
        std::valarray<T> temp = *this;
        this->assign(length, std::forward<const T>(val));
        std::move(begin(temp), min(end(temp), next(begin(temp), length)), begin(*this));
    }
};

} // namespace Lib
