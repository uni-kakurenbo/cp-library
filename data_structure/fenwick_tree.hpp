#pragma once


#include <cassert>
#include <vector>
#include <iterator>
#include <utility>
#include <type_traits>
#include <concepts>
#include <ranges>
#include <bit>


#include "internal/dev_env.hpp"
#include "internal/concepts.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"
#include "internal/unconstructible.hpp"

#include "snippet/iterations.hpp"
#include "numeric/bit.hpp"

#include "action/base.hpp"
#include "algebraic/internal/concepts.hpp"


namespace uni {

namespace internal {

namespace fenwick_tree_impl {


// Thanks to: atcoder::fenwick_tree
template<algebraic::internal::monoid Operand>
struct core {
    using operand = Operand;
    using size_type = internal::size_t;

  private:
    size_type _n = 0, _bit_ceil = 0;
    std::vector<operand> _data;

    inline void _init() noexcept(NO_EXCEPT) {
        FOR(i, 1, this->_n) {
            size_type j = i + (i & -i);
            if(j <= this->_n) this->_data[j-1] = this->_data[j-1] + this->_data[i-1];
        }
    }

  public:
    core() noexcept(NO_EXCEPT) {}

    explicit core(const size_type n) noexcept(NO_EXCEPT)
      : _n(n), _bit_ceil(std::bit_ceil<std::make_unsigned_t<size_type>>(n)), _data(n, operand{})
    {}

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }


    template<std::input_iterator I, std::sentinel_for<I> S>
    inline void assign(I first, S last) noexcept(NO_EXCEPT) {
        if constexpr(std::sized_sentinel_for<S, I>) {
            assert(std::ranges::distance(first, last) == this->size());
        }
        std::ranges::copy(first, last, std::ranges::begin(this->_data));
        this->_init();
    }


    inline void apply(size_type p, const operand& x) noexcept(NO_EXCEPT) {
        for(p++; p<=this->_n; p += p & -p) this->_data[p-1] = this->_data[p-1] + x;
    }

    inline void set(const size_type p, const operand& x) noexcept(NO_EXCEPT) {
        assert(this->get(p) == this->fold(p, p+1));
        this->apply(p, x + -this->get(p));
    }

    inline operand fold(size_type r) const noexcept(NO_EXCEPT) {
        operand s = operand{};
        for(; r>0; r -= r & -r) s = s + this->_data[r-1];
        return s;
    }
    inline operand fold(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        operand s = operand{};
        for(; l < r; r -= r & -r) s = s + this->_data[r-1];
        for(; r < l; l -= l & -l) s = s + -this->_data[l-1];
        return s;
    }

    inline operand get(size_type p) const noexcept(NO_EXCEPT) {
        return this->fold(p, p+1);
    }

    template<class F>
    inline size_type max_right(size_type l, const F& f) const noexcept(NO_EXCEPT)
        requires algebraic::internal::invertible<operand>
    {
        assert(0 <= l && l <= this->_n);
        assert(f(operand{}));
        if(l == this->_n) return this->_n;
        operand inv = -this->fold(l);
        size_type p = 0, q = this->_bit_ceil;
        for(size_type k=q; k>0; k >>= 1) {
            if(p+k <= this->_n and f(this->_data[p+k-1] + inv)) {
                inv = inv + this->_data[(p+=k)-1];
            }
        }
        return p;
    }

    template<class F> inline size_type min_left(size_type r, const F& f) const noexcept(NO_EXCEPT)
        requires algebraic::internal::invertible<operand>
    {
        assert(0 <= r && r <= this->_n);
        assert(f(operand{}));
        if(r == 0) return 0;
        operand acc = this->fold(r);
        size_type p = 0, q = std::bit_ceil<std::make_unsigned_t<size_type>>(r);
        for(size_type k=q; k>0; k >>= 1) {
            if(p+k < r and !f(acc + -this->_data[p+k-1])) {
                acc = acc + -this->_data[(p+=k)-1];
            }
        }
        if(p == 0 and f(acc)) return 0;
        return p + 1;
    }
};


} // namespace fenwick_tree_impl

} // namespace internal


template<class Value>
struct fenwick_tree : internal::unconstructible {};


template<algebraic::internal::monoid Monoid>
    requires algebraic::internal::commutative<Monoid>
struct fenwick_tree<Monoid> : internal::fenwick_tree_impl::core<Monoid> {
  private:
    using core = typename internal::fenwick_tree_impl::core<Monoid>;

    core _impl;

  public:
    using value_type = typename core::operand;
    using size_type = typename core::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->_impl.size() + p : p;
    }

  public:
    fenwick_tree() noexcept(NO_EXCEPT) : _impl() {}

    explicit fenwick_tree(const size_type n) noexcept(NO_EXCEPT) : _impl(n) {}
    explicit fenwick_tree(const size_type n, const value_type& v) noexcept(NO_EXCEPT) : _impl(n) { this->_impl.fill(v); }

    template<std::convertible_to<value_type> T>
    fenwick_tree(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT) : fenwick_tree(ALL(init_list)) {}

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    explicit fenwick_tree(I first, S last) noexcept(NO_EXCEPT)
      : fenwick_tree(static_cast<size_type>(std::ranges::distance(first, last)))
    { this->assign(first, last); }

    template<std::ranges::input_range R>
    explicit fenwick_tree(R&& range) noexcept(NO_EXCEPT) : fenwick_tree(ALL(range)) {}


    template<std::convertible_to<value_type> T>
    inline auto& assign(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT){ return this->assign(ALL(init_list)); }

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& assign(I first, S last) noexcept(NO_EXCEPT) {
        this->_impl.assign(first, last);
        return *this;
    }

    template<std::ranges::input_range R>
    inline auto& assign(R&& range) noexcept(NO_EXCEPT) { return this->assign(ALL(range)); }

    inline auto& fill(const value_type& v = {}) noexcept(NO_EXCEPT) {
        std::fill(this->data(), this->data() + this->_impl.size(), v);
        this->_init();
        return *this;
    }

    inline auto size() const noexcept(NO_EXCEPT) { return this->_impl.size(); }
    inline bool empty() const noexcept(NO_EXCEPT) { return this->_impl.size() == 0; }

    struct point_reference : internal::point_reference<fenwick_tree> {
        point_reference(fenwick_tree *const super, const size_type p) noexcept(NO_EXCEPT)
          : internal::point_reference<fenwick_tree>(super, super->_positivize_index(p))
        {
            assert(0 <= this->_pos && this->_pos < this->_super->size());
        }

        operator value_type() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        value_type val() const noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }

        inline point_reference& set(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator+=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<fenwick_tree> {
        range_reference(fenwick_tree *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<fenwick_tree>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline value_type fold() noexcept(NO_EXCEPT) {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            if(this->_begin == 0) return this->_super->fold(this->_end);
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto& apply(const size_type p, const value_type& x) noexcept(NO_EXCEPT) {
        assert(0 <= p && p < this->_impl.size());
        this->_impl.apply(p, x);
         return *this;
    }

    inline auto& set(const size_type p, const value_type& x) noexcept(NO_EXCEPT)
        requires algebraic::internal::invertible<value_type>
    {
        assert(0 <= p && p < this->_impl.size());
        this->_impl.set(p, x);
         return *this;
    }

    inline value_type get(const size_type p) const noexcept(NO_EXCEPT)
        requires algebraic::internal::invertible<value_type>
    {
        assert(0 <= p && p < this->_impl.size());
        return this->_impl.get(p);
    }

    inline point_reference operator[](const size_type p) noexcept(NO_EXCEPT) { return point_reference(this, p); }

    inline const range_reference operator()(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        return range_reference(this, l, r);
    }

    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) {
        return range_reference(this, l, r);
    }

    inline value_type fold(const size_type l, const size_type r) const noexcept(NO_EXCEPT)
        requires algebraic::internal::invertible<value_type>
    {
        assert(0 <= l && l <= r && r <= this->_impl.size());
        return this->_impl.fold(l, r);
    }

    inline value_type fold(const size_type r) const noexcept(NO_EXCEPT) {
        assert(0 <= r && r <= this->_impl.size());
        return this->_impl.fold(r);
    }

    inline value_type fold() const noexcept(NO_EXCEPT) {
        return this->_impl.fold(this->_impl.size());
    }

    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type, const fenwick_tree, iterator>;

  public:
    struct iterator : iterator_interface {
        iterator() noexcept = default;
        iterator(const fenwick_tree *const ref, const size_type p) noexcept(NO_EXCEPT) : iterator_interface(ref, p) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->_impl.size()); }
};


template<actions::internal::operatable_action Action>
struct fenwick_tree<Action> : fenwick_tree<typename Action::operand> {
    using fenwick_tree<typename Action::operand>::fenwick_tree;
};


} // namespace uni
