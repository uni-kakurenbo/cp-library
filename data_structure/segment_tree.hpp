#pragma once


#include <algorithm>
#include <vector>

#include <type_traits>

#include <atcoder/internal_bit>

#include "internal/dev_assert.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "snippet/iterations.hpp"

#include "data_structure/internal/is_action.hpp"
#include "data_structure/internal/is_monoid.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace segment_tree_lib {


// Thanks to: atcoder::segtree
template<class S> struct base {
    using size_type = internal::size_t;

  protected:
    size_type _n, _size, _depth;
    S* _data;

    inline void update(const size_type k) { this->_data[k] = this->_data[k << 1] * this->_data[k << 1 | 1]; }

  protected:
    explicit base(const size_type n = 0) : _n(n), _depth(atcoder::internal::ceil_pow2(n)) {
        this->_size = 1 << this->_depth;
        this->_data = new S[this->_size << 1]();
    }
    ~base() { delete[] this->_data; }

  public:
    inline size_type size() const { return this->_n; }
    inline size_type allocated() const { return this->_size; }
    inline size_type depth() const { return this->_depth; }

    inline void set(size_type p, const S& x) {
        p += this->_size;
        this->_data[p] = x;
        FOR(i, 1, this->_depth) this->update(p >> i);
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
        return this->max_right(l, [](S x) { return f(x); });
    }
    template<class F> inline size_type max_right(size_type l, F f) const {
        assert(0 <= l && l <= _n);
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
        return this->min_left(r, [](S x) { return f(x); });
    }
    template<class F> inline size_type min_left(size_type r, F f) const {
        assert(0 <= r && r <= _n);
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

    explicit core(const size_type n, const value_type& v) : core(n) {
        REP(p, this->_n) this->_data[this->_size + p] = v;
        REPD(p, 1, this->_size) this->update(p);
    }

    template<class T> core(const std::initializer_list<T>& init_list) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) : core(std::distance(first, last)) {
        size_type p = 0;
        for(auto itr=first; itr!=last; ++itr, ++p) this->_data[this->_size + p] = monoid(*itr);
        REPD(p, 1, this->_size) this->update(p);
    }

    inline void set(const size_type p, const value_type& x) {
        dev_assert(0 <= p and p < this->size());
        this->base::set(p, x);
    }

    inline value_type get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->base::prod(p, p+1).val();
    }
    inline value_type operator[](const size_type p) const { return this->get(p); }

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
        iterator(const core *const ref, const size_type p) : internal::container_iterator_interface<value_type,core>(ref, p) {}

        inline value_type operator*() const override { return this->ref()->get(this->p()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};

template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand_monoid> {
    using action = Action;
    using core<typename action::operand_monoid>::core;
    static_assert(action::tags.bits() == 0 or action::tags.has(actions::flags::segment_tree));
};


} // namespace segment_tree_lib

} // namespace internal


template<class monoid> struct segment_tree : internal::segment_tree_lib::core<monoid> {
    using internal::segment_tree_lib::core<monoid>::core;
};


} // namespace lib
