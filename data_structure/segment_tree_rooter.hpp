#pragma once


#include <concepts>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace uni {


template<std::integral SizeType>
struct segment_tree_rooter {
    using size_type = SizeType;

  private:
    size_type _n = 0;

  public:
    segment_tree_rooter() noexcept = default;

    explicit segment_tree_rooter(const size_type n) noexcept(NO_EXCEPT) : _n(n) {};

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type allocated() const noexcept(NO_EXCEPT) { return (this->_n << 1) - 1; }

    template<std::invocable<size_type> F>
    void range_to_nodes(size_type l, size_type r, F f) noexcept(NO_EXCEPT) {
        l += this->_n;
        r += this->_n;

        while(l < r) {
            if(l & 1) f(l++ - 1);
            if(r & 1) f(--r - 1);
            l >>= 1;
            r >>= 1;
        }
    }

    size_type point_to_node(const size_type p) noexcept(NO_EXCEPT) { return this->_n + p - 1; }

    template<std::invocable<size_type> F>
    void point_to_path(size_type p, F f) noexcept(NO_EXCEPT) {
        p += this->_n;
        while(p > 0) f(p - 1), p >>= 1;
    }
};

}
