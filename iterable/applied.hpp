#pragma once


#include <algorithm>
#include <vector>
#include <iterator>

#include "internal/dev_env.hpp"


namespace lib {


template<class I, class F, class C = std::vector<typename std::iterator_traits<I>::value_type>>
inline auto applied(const I first, const I last, F&& func) noexcept(DEV_ENV) {
    C res(first, last);
    func(std::begin(res), std::end(res));
    return res;
}

template<class V, class F, class C = V>
inline auto applied(V v, F&& func) noexcept(DEV_ENV) {
    return applied<typename V::iterator,F,C>(std::begin(v), std::end(v), func);
}


template<class I>
inline auto sorted(const I first, const I last) noexcept(DEV_ENV) {
    return applied(first, last, std::sort<I>);
}

template<class V>
inline auto sorted(V v) noexcept(DEV_ENV) {
    return applied(v, std::sort<typename V::iterator>);
}


template<class I>
inline auto reversed(const I first, const I last) noexcept(DEV_ENV) {
    return applied(first, last, std::reverse<I>);
}

template<class V>
inline auto reversed(V v) noexcept(DEV_ENV) {
    return applied(v, std::reverse<typename V::iterator>);
}


} // namespace lib
