#pragma once

#include "data_structure/core/implicit_treap.hpp"

namespace lib {


template <class T0, class T1>
struct range_add_range_min : implicit_treap<T0, T1> {
    using size_t = typename implicit_treap<T0, T1>::size_t;

    template<class... Args>
    explicit range_add_range_min(Args&&... args) : implicit_treap<T0, T1>::implicit_treap(std::numeric_limits<T0>::max(), 0) {
        this->initialize(std::forward<Args>(args)...);
    }

    inline T0 f0(const T0 x, const T0 y) const override { return std::min(x, y); }
    inline T1 f1(const T1 x, const T1 y) const override { return x + y; }
    inline T0 g(const T0 x, const T1 y) const override { return x + y; }
    inline T1 p(const T1 x, const size_t) const override { return x; }

    inline auto add(const size_t first, const size_t last, const T1& val) { return this->apply(first, last, val); }
    inline auto add(const size_t pos, const T1& val) { return this->apply(pos, val); }
    inline auto add(const T1& val) { return this->apply(val); }

    inline auto min(const size_t first, const size_t last) { return this->prod(first, last); }
    inline auto min() { return this->prod(); }
};


} // namespace lib
