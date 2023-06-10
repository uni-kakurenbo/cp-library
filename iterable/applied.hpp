#pragma once


#include <algorithm>
#include <vector>
#include <iterator>

#include "internal/dev_env.hpp"


namespace lib {


template<class I, class F, class C = std::vector<typename std::iterator_traits<I>::value_type>>
inline auto applied(const I first, const I last, F&& func) noexcept(NO_EXCEPT) {
    C res(first, last);
    func(std::begin(res), std::end(res));
    return res;
}

template<class V, class F, class C = V>
inline auto applied(V v, F&& func) noexcept(NO_EXCEPT) {
    return applied<typename V::iterator,F,C>(std::begin(v), std::end(v), func);
}


template<class I>
inline auto sorted(const I first, const I last) noexcept(NO_EXCEPT) {
    return applied(first, last, std::sort<I>);
}

template<class V>
inline auto sorted(V v) noexcept(NO_EXCEPT) {
    return applied(v, std::sort<typename V::iterator>);
}


template<class I>
inline auto reversed(const I first, const I last) noexcept(NO_EXCEPT) {
    return applied(first, last, std::reverse<I>);
}

template<class V>
inline auto reversed(V v) noexcept(NO_EXCEPT) {
    return applied(v, std::reverse<typename V::iterator>);
}

template<class V>
inline auto unique(V v) noexcept(NO_EXCEPT) {
    std::sort(std::begin(v), std::end(v));
    v.erase(std::unique(std::begin(v), std::end(v)), std::end(v));
    return v;
}


} // namespace lib
