#pragma once


#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include <atcoder/internal_bit>

#include "snippet/iterations.hpp"

#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/dev_assert.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace lazy_segment_tree_lib {

// Thanks to: atcoder::lazy_segtree
template<class S, class F, S (*map)(const S&, const F&), F (*fold)(const F&, const internal::size_t)>
struct base {
    using size_type = internal::size_t;

//   protected:
  public:
    size_type _n, _size, _depth;
    mutable size_type* _lengths;
    mutable S* _values;
    mutable F* _lazy;

    inline void update(const size_type k) {
        this->_values[k] = this->_values[k << 1] * this->_values[k << 1 | 1];
    }
    inline void all_apply(const size_type k, const F& f) const {
        this->_values[k] = map(this->_values[k], fold(f, this->_lengths[k]));
        if(k < this->_size) this->_lazy[k] = f * this->_lazy[k];
    }
    inline void push(const size_type k) const {
        this->all_apply(k << 1, this->_lazy[k]);
        this->all_apply(k << 1 | 1, this->_lazy[k]);
        this->_lazy[k] = {};
    }

  protected:
    explicit base(const size_type n = 0) : _n(n), _depth(atcoder::internal::ceil_pow2(_n)) {
        this->_size = 1 << this->_depth;
        this->_lengths = new size_type[2*this->_size]();
        this->_values = new S[2*this->_size]();
        this->_lazy = new F[this->_size]();
    }
    ~base() {
        delete[] this->_lengths;
        delete[] this->_values;
        delete[] this->_lazy;
    }

    inline void initialize() {
        REPD(pos, 1, this->_size) {
            this->_lengths[pos] = this->_lengths[pos << 1] + this->_lengths[pos << 1 | 1];
            this->update(pos);
        }
    }

  public:
    inline size_type size() const { return this->_n; }
    inline size_type allocated() const { return this->_size; }
    inline size_type depth() const { return this->_depth; }

    inline void set(size_type pos, const S& x) {
        pos += this->_size;
        FORD(i, 1, this->_depth) this->push(pos >> i);
        this->_values[pos] = x;
        FOR(i, 1, this->_depth) this->update(pos >> i);
    }

    inline S get(size_type pos) const {
        pos += this->_size;
        FORD(i, 1, this->_depth) this->push(pos >> i);
        return this->_values[pos];
    }

    inline S prod(size_type l, size_type r) const {
        if(l == r) return {};

        l += this->_size;
        r += this->_size;

        FORD(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->push(l >> i);
            if(((r >> i) << i) != r) this->push((r - 1) >> i);
        }

        S sml, smr;
        while(l < r) {
            if(l & 1) sml = sml * this->_values[l++];
            if(r & 1) smr = this->_values[--r] * smr;
            l >>= 1;
            r >>= 1;
        }

        return sml * smr;
    }

    inline S all_prod() const { return this->_values[1]; }

    inline void apply(size_type pos, const F& f) {
        pos += this->_size;
        FORD(i, 1, this->_depth) this->push(pos >> i);
        this->_values[pos] = map(this->_values[pos], fold(f, this->_lengths[pos]));
        FOR(i, 1, this->_depth) this->update(pos >> i);
    }
    inline void apply(size_type l, size_type r, const F& f) {
        if(l == r) return;

        l += this->_size;
        r += this->_size;

        FORD(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->push(l >> i);
            if(((r >> i) << i) != r) this->push((r - 1) >> i);
        }

        {
            size_type l2 = l, r2 = r;
            while(l < r) {
                if(l & 1) this->all_apply(l++, f);
                if(r & 1) this->all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        FOR(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->update(l >> i);
            if(((r >> i) << i) != r) this->update((r - 1) >> i);
        }
    }

    template<bool (*g)(S)> inline size_type max_right(size_type l) const {
        return this->max_right(l, [](S x) { return g(x); });
    }
    template<class G> inline size_type max_right(size_type l, G g) const {
        dev_assert(0 <= l && l <= _n);
        dev_assert(g({}));
        if(l == _n) return _n;
        l += this->_size;
        FORD(i, 1, this->_depth) this->push(l >> i);
        S sm;
        do {
            while(l % 2 == 0) l >>= 1;
            if(!g(sm * this->_values[l])) {
                while(l < this->_size) {
                    this->push(l);
                    l = (2 * l);
                    if(g(sm * this->_values[l])) {
                        sm = sm * this->_values[l];
                        l++;
                    }
                }
                return l - this->_size;
            }
            sm = sm * this->_values[l];
            l++;
        } while((l & -l) != l);
        return _n;
    }

    template<bool (*g)(S)> inline size_type min_left(size_type r) const {
        return min_left(r, [](S x) { return g(x); });
    }
    template<class G> inline size_type min_left(size_type r, G g) const {
        dev_assert(0 <= r && r <= _n);
        dev_assert(g({}));
        if(r == 0) return 0;
        r += this->_size;
        FORD(i, 1, this->_depth) this->push((r - 1) >> i);
        S sm;
        do {
            r--;
            while(r > 1 && (r % 2)) r >>= 1;
            if(!g(this->_values[r] * sm)) {
                while(r < this->_size) {
                    this->push(r);
                    r = (2 * r + 1);
                    if(g(this->_values[r] * sm)) {
                        sm = this->_values[r] * sm;
                        r--;
                    }
                }
                return r + 1 - this->_size;
            }
            sm = this->_values[r] * sm;
        } while((r & -r) != r);
        return 0;
    }
};


template<class Action>
struct core : base<typename Action::operand_monoid, typename Action::operator_monoid, Action::map, Action::fold> {
  public:
    using action = Action;

    using operand_monoid = typename action::operand_monoid;
    using operator_monoid = typename action::operator_monoid;

  private:
    using base = lazy_segment_tree_lib::base<operand_monoid, operator_monoid, action::map, action::fold>;

  public:
    using value_type = typename operand_monoid::value_type;
    using action_type = typename operator_monoid::value_type;

    using size_type = typename base::size_type;

  protected:
    inline void _validate_index_in_right_open(const size_t _pos) const {
        dev_assert(0 <= _pos and _pos < this->size());
    }
    inline void _validate_index_in_closed(const size_t _pos) const {
        dev_assert(0 <= _pos and _pos <= this->size());
    }
    inline void _validate_rigth_open_interval(const size_t _l, const size_t _r) const {
        dev_assert(0 <= _l and _l <= _r and _r <= this->size());
    }

    inline size_t _positivize_index(const size_t pos) const {
        return pos < 0 ? this->size() + pos : pos;
    }

  public:
    explicit core(const size_type n = 0, const value_type& v = {}) : base(n) {
        static_assert(action::tags.bits() == 0 or action::tags.has(actions::flags::lazy_segment_tree));
        REP(pos, 0, this->_n) this->_lengths[this->_size + pos] = 1, this->_values[this->_size + pos] = v;
        this->initialize();
    }

    core(const std::initializer_list<value_type>& init_list) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) : base(std::distance(first, last)) {
        static_assert(action::tags.bits() == 0 or action::tags.has(actions::flags::lazy_segment_tree));
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) {
            this->_lengths[this->_size + pos] = 1, this->_values[this->_size + pos] = operand_monoid(*itr);
        }
        this->initialize();
    }

    inline void set(const size_type pos, const value_type& value) {
        pos = this->_positivize_index(pos), this->_validate_index_in_right_open(pos);
        this->base::set(value);
    }

    inline void apply(size_t first, size_t last, const action_type& value) {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        this->base::apply(first, last, value);
    }
    inline void apply(const size_t pos, const action_type& value) { this->apply(pos, pos+1, value); }
    inline void apply(const action_type& value) { this->apply(0, this->size(), value); }


    inline value_type get(size_t pos) const {
        pos = this->_positivize_index(pos);
        this->_validate_index_in_right_open(pos);
        return this->base::get(pos).val();
    }
    inline value_type operator[](const size_t pos) const { return this->get(pos); }

    inline value_type prod(size_t first, size_t last) const {
        first = this->_positivize_index(first), last = this->_positivize_index(last);
        this->_validate_rigth_open_interval(first, last);
        return this->base::prod(first, last).val();
    }
    inline value_type prod() const { return this->prod(0, this->size()); }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_t pos) : internal::container_iterator_interface<value_type,core>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lazy_segment_tree_lib

} // namespace internal


template<class action> struct lazy_segment_tree : internal::lazy_segment_tree_lib::core<action> {
    using internal::lazy_segment_tree_lib::core<action>::core;
};


} // namespace lib
