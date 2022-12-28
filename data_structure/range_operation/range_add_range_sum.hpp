#pragma once

#include "data_structure/core/implicit_treap.hpp"

namespace Lib {


template <class T0, class T1>
struct RangeAddRangeSum : ImplicitTreap<T0, T1> {
    using Size = typename ImplicitTreap<T0, T1>::Size;

    template<class... Args>
    explicit RangeAddRangeSum(Args&&... args) : ImplicitTreap<T0, T1>::ImplicitTreap(0, 0) {
        this->initialize(std::forward<Args>(args)...);
    }

    inline T0 f0(const T0 x, const T0 y) const override { return x + y; }
    inline T1 f1(const T1 x, const T1 y) const override { return x + y; }
    inline T0 g(const T0 x, const T1 y) const override { return x + y; }
    inline T1 p(const T1 x, const Size len) const override { return x * len; }

    inline auto add(const Size first, const Size last, const T1& val) { return this->apply(first, last, val); }
    inline auto add(const Size pos, const T1& val) { return this->apply(pos, val); }
    inline auto add(const T1& val) { return this->apply(val); }

    inline auto sum(const Size first, const Size last) { return this->prod(first, last); }
    inline auto sum() { return this->prod(); }
};


} // namespace Lib
