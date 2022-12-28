#pragma once

#include "data_structure/core/implicit_treap.hpp"

namespace Lib {


template <class T0, class T1>
struct RangeSetRangeMax : ImplicitTreap<T0, T1> {
    using Size = typename ImplicitTreap<T0, T1>::Size;

    template<class... Args>
    explicit RangeSetRangeMax(Args&&... args)
    : ImplicitTreap<T0, T1>::ImplicitTreap(std::numeric_limits<T0>::min(), std::numeric_limits<T1>::max()) {
        this->initialize(std::forward<Args>(args)...);
    }

    inline T0 f0(const T0 x, const T0 y) const override { return std::max(x, y); }
    inline T1 f1(const T1 x, const T1 y) const override { return y == std::numeric_limits<T1>::max() ? x : y; }
    inline T0 g(const T0 x, const T1 y) const override { return y == std::numeric_limits<T1>::max() ? x : y; }
    inline T1 p(const T1 x, const Size) const override { return x; }

    inline auto set(const Size first, const Size last, const T1& val) { return this->apply(first, last, val); }
    inline auto set(const Size pos, const T1& val) { return this->apply(pos, val); }
    inline auto set(const T1& val) { return this->apply(val); }

    inline auto max(const Size first, const Size last) { return this->prod(first, last); }
    inline auto max() { return this->prod(); }
};


} // namespace Lib
