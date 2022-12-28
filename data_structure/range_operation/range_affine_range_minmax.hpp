#pragma once

#include "data_structure/core/implicit_treap.hpp"

namespace Lib {


template<class T>
struct RangeAffineRangeMinMax : ImplicitTreap<std::pair<T,T>, std::pair<T,T>> {
    using Size = typename ImplicitTreap<std::pair<T,T>, std::pair<T,T>>::Size;

    using T0 = std::pair<T,T>;  // {std::min, max}
    using T1 = std::pair<T,T>;  // first * x + second

    template<class... Args>
    explicit RangeAffineRangeMinMax(Args&&... args)
    : ImplicitTreap<T0,T1>::ImplicitTreap({ std::numeric_limits<T>::max(), std::numeric_limits<T>::min() }, { 1, 0 }) {
        this->initialize(std::forward<Args>(args)...);
    }


    inline T0 f0(const T0 x, const T0 y) const override { return { std::min(x.first, y.first), std::max(x.second, y.second) }; }
    inline T1 f1(const T1 x, const T1 y) const override { return { x.first * y.first, x.second * y.first + y.second }; }
    inline T0 g(const T0 x, const T1 y) const override {
        T0 ret = { x.first * y.first + y.second, x.second * y.first + y.second };
        if (y.first < 0) std::swap(ret.first, ret.second);
        return ret;
    }
    inline T1 p(const T1 x, const Size) const override { return x; }


    inline auto affine(const Size first, const Size last, const T& a, const T& b) { return this->apply(first, last, { a, b }); }
    inline auto affine(const Size pos, const T& a, const T& b) { return this->apply(pos, { a, b }); }
    inline auto affine(const T& a, const T& b) { return this->apply({ a, b }); }

    inline auto set(const Size first, const Size last, const T& val) { return this->apply(first, last, { 0, val }); }
    inline auto set(const Size pos, const T& val) { return this->apply(pos, { 0, val }); }
    inline auto set(const T& val) { return this->apply({ 0, val }); }

    inline auto add(const Size first, const Size last, const T& val) { return this->apply(first, last, { 1, val }); }
    inline auto add(const Size pos, const T& val) { return this->apply(pos, { 1, val }); }
    inline auto add(const T& val) { return this->apply({ 1, val }); }

    inline auto times(const Size first, const Size last, const T& val) { return this->apply(first, last, { val, 0 }); }
    inline auto times(const Size pos, const T& val) { return this->apply(pos, { val, 0 }); }
    inline auto times(const T& val) { return this->apply({ val, 0 }); }


    inline auto minmax(const Size first, const Size last) { return this->prod(first, last); }
    inline auto minmax() { return this->prod(); }

    inline auto min(const Size first, const Size last) { return this->prod(first, last).first; }
    inline auto min() { return this->prod().first; }

    inline auto max(const Size first, const Size last) { return this->prod(first, last).second; }
    inline auto max() { return this->prod().second; }
};


} // namespace Lib
