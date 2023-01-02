#pragma once


#include <algorithm>
#include <vector>

#include <type_traits>

#include <atcoder/internal_bit>

#include "internal/dev_assert.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "data_structure/internal/is_action.hpp"
#include "data_structure/internal/is_monoid.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace segment_tree_lib {


// Thanks to: atcoder::segtreeZ
template<class S> struct base {
    using size_type = internal::size_t;

  private:
    size_type _n, _size, _depth;
    std::vector<S> _data;

    inline void update(size_type k) { this->_data[k] = this->_data[2 * k] * this->_data[2 * k + 1]; }

  public:
    base() : base(0) {}
    explicit base(const size_type n) : base(std::vector<S>(n)) {}
    explicit base(const std::vector<S>& v) : _n(size_type(v.size())) {
        this->_depth = atcoder::internal::ceil_pow2(this->_n);
        this->_size = 1 << this->_depth;
        this->_data = std::vector<S>(2 * this->_size);
        for (size_type i = 0; i < this->_n; i++) this->_data[this->_size + i] = v[i];
        for (size_type i = this->_size - 1; i >= 1; i--) {
            this->update(i);
        }
    }

    inline size_type size() const { return this->_n; }
    inline size_type capacity() const { return this->_size; }
    inline size_type depth() const { return this->_depth; }

    inline void set(size_type p, S x) {
        p += this->_size;
        this->_data[p] = x;
        for (size_type i = 1; i <= this->_depth; i++) this->update(p >> i);
    }

    inline S get(size_type p) const {
        return this->_data[p + this->_size];
    }

    inline S prod(size_type l, size_type r) const {
        S sml, smr;
        l += this->_size;
        r += this->_size;

        while (l < r) {
            if (l & 1) sml = sml * this->_data[l++];
            if (r & 1) smr = this->_data[--r] * smr;
            l >>= 1;
            r >>= 1;
        }
        return sml * smr;
    }

    inline S all_prod() const { return this->_data[1]; }

    template<bool (*f)(S)> inline size_type max_right(size_type l) const {
        return max_right(l, [](S x) { return f(x); });
    }
    template<class F> inline size_type max_right(size_type l, F f) const {
        assert(f(S{}));
        if (l == this->_n) return this->_n;
        l += this->_size;
        S sm;
        do {
            while (l % 2 == 0) l >>= 1;
            if (!f(sm * this->_data[l])) {
                while (l < this->_size) {
                    l = (2 * l);
                    if (f(sm * this->_data[l])) {
                        sm = sm * this->_data[l];
                        ++l;
                    }
                }
                return l - this->_size;
            }
            sm = sm * this->_data[l];
            ++l;
        } while ((l & -l) != l);
        return this->_n;
    }

    template<bool (*f)(S)> inline size_type min_left(size_type r) const {
        return min_left(r, [](S x) { return f(x); });
    }
    template<class F> inline size_type min_left(size_type r, F f) const {
        assert(f(S()));
        if (r == 0) return 0;
        r += this->_size;
        S sm;
        do {
            --r;
            while (r > 1 and (r % 2)) r >>= 1;
            if (!f(this->_data[r] * sm)) {
                while (r < this->_size) {
                    r = (2 * r + 1);
                    if (f(this->_data[r] * sm)) {
                        sm = this->_data[r] * sm;
                        --r;
                    }
                }
                return r + 1 - this->_size;
            }
            sm = this->_data[r] * sm;
        } while ((r & -r) != r);
        return 0;
    }
};

template<class, class = std::void_t<>> struct core {};

template<class Monoid>
struct core<Monoid, std::void_t<typename internal::is_monoid_t<Monoid>>> : base<Monoid> {
  public:
    using monoid = Monoid;

  private:
    using base = typename segment_tree_lib::base<monoid>;

  public:
    using value_type = typename monoid::value_type;
    using size_type = typename base::size_type;

  public:
    explicit core(const size_type n = 0) : base(n) {}

    explicit core(const size_type n, const value_type& v) : base(n) { if(v != 0) REP(i, n) this->base::set(i, v); }

    explicit core(const std::initializer_list<value_type>& init_list) : core(ALL(init_list)) {}

    template<class I, std::enable_if_t<std::is_same_v<value_type, typename std::iterator_traits<I>::value_type>>* = nullptr>
    explicit core(const I first, const I last) : core(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->base::set(pos, *itr);
    }

    inline void set(const size_type p, const value_type& x) {
        dev_assert(0 <= p and p < this->size());
        this->base::set(p, x);
    }

    inline value_type get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->base::prod(p, p+1).val();
    }
    inline value_type operator[](const size_type pos) const { return this->get(pos); }

    inline value_type prod(const size_type l, const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
        return this->base::prod(l, r).val();
    }
    inline value_type prod(const size_type r) const {
        dev_assert(0 <= r and r <= this->size());
        return this->base::prod(0, r).val();
    }
    inline value_type prod() const {
        return this->base::all_prod().val();
    }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_type pos) : internal::container_iterator_interface<value_type,core>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};

template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand_monoid> {
    using action = Action;
    using core<typename action::operand_monoid>::core;
    static_assert(action::tags.has(actions::flags::segment_tree));
};


} // namespace segment_tree_lib

} // namespace internal


template<class monoid> struct segment_tree : internal::segment_tree_lib::core<monoid> {
    using internal::segment_tree_lib::core<monoid>::core;
};


} // namespace lib
