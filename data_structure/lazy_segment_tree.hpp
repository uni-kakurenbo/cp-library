#pragma once


#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>

#include <atcoder/internal_bit>

#include "snippet/iterations.hpp"

#include "numeric/bit.hpp"

#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace lazy_segment_tree_impl {

// Thanks to: atcoder::lazy_segtree
template<class S, class F, S (*_map)(const S&, const F&), F (*_fold)(const F&, const internal::size_t)>
struct base {
    using size_type = internal::size_t;

  public:
    size_type _n, _size, _depth;
    mutable size_type* _lengths;
    mutable S* _values;
    mutable F* _lazy;

    inline void update(const size_type p) {
        this->_values[p] = this->_values[p << 1] + this->_values[p << 1 | 1];
    }
    inline void all_apply(const size_type p, const F& f) const {
        this->_values[p] = _map(this->_values[p], _fold(f, this->_lengths[p]));
        if(p < this->_size) this->_lazy[p] = f + this->_lazy[p];
    }
    inline void push(const size_type p) const {
        this->all_apply(p << 1, this->_lazy[p]);
        this->all_apply(p << 1 | 1, this->_lazy[p]);
        this->_lazy[p] = {};
    }

  protected:
    explicit base(const size_type n = 0) : _n(n), _depth(bit_width<unsigned>(_n)) {
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
        REPD(p, 1, this->_size) {
            this->_lengths[p] = this->_lengths[p << 1] + this->_lengths[p << 1 | 1];
            this->update(p);
        }
    }

  public:
    inline size_type size() const { return this->_n; }
    inline size_type allocated() const { return this->_size; }
    inline size_type depth() const { return this->_depth; }

    inline void set(size_type p, const S& x) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->push(p >> i);
        this->_values[p] = x;
        FOR(i, 1, this->_depth) this->update(p >> i);
    }

    inline S get(size_type p) const {
        p += this->_size;
        FORD(i, 1, this->_depth) this->push(p >> i);
        return this->_values[p];
    }

    inline S fold(size_type l, size_type r) const {
        if(l == r) return {};

        l += this->_size;
        r += this->_size;

        FORD(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->push(l >> i);
            if(((r >> i) << i) != r) this->push((r - 1) >> i);
        }

        S sml, smr;
        while(l < r) {
            if(l & 1) sml = sml + this->_values[l++];
            if(r & 1) smr = this->_values[--r] + smr;
            l >>= 1;
            r >>= 1;
        }

        return sml + smr;
    }

    inline S fold_all() const { return this->_values[1]; }

    inline void apply(size_type p, const F& f) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->push(p >> i);
        this->_values[p] = _map(this->_values[p], _fold(f, this->_lengths[p]));
        FOR(i, 1, this->_depth) this->update(p >> i);
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
        assert(0 <= l && l <= _n);
        assert(g({}));
        if(l == _n) return _n;
        l += this->_size;
        FORD(i, 1, this->_depth) this->push(l >> i);
        S sm;
        do {
            while(l % 2 == 0) l >>= 1;
            if(!g(sm + this->_values[l])) {
                while(l < this->_size) {
                    this->push(l);
                    l = (2 * l);
                    if(g(sm + this->_values[l])) {
                        sm = sm + this->_values[l];
                        l++;
                    }
                }
                return l - this->_size;
            }
            sm = sm + this->_values[l];
            l++;
        } while((l & -l) != l);
        return _n;
    }

    template<bool (*g)(S)> inline size_type min_left(size_type r) const {
        return min_left(r, [](S x) { return g(x); });
    }
    template<class G> inline size_type min_left(size_type r, G g) const {
        assert(0 <= r && r <= _n);
        assert(g({}));
        if(r == 0) return 0;
        r += this->_size;
        FORD(i, 1, this->_depth) this->push((r - 1) >> i);
        S sm;
        do {
            r--;
            while(r > 1 && (r % 2)) r >>= 1;
            if(!g(this->_values[r] + sm)) {
                while(r < this->_size) {
                    this->push(r);
                    r = (2 * r + 1);
                    if(g(this->_values[r] + sm)) {
                        sm = this->_values[r] + sm;
                        r--;
                    }
                }
                return r + 1 - this->_size;
            }
            sm = this->_values[r] + sm;
        } while((r & -r) != r);
        return 0;
    }
};


template<class Action>
struct core : base<typename Action::operand, typename Action::operation, Action::map, Action::fold> {
    static_assert(
        Action::tags.none() or
        Action::tags.has(actions::flags::range_folding, actions::flags::range_operation)
    );

  public:
    using action = Action;

    using operand = typename action::operand;
    using operation = typename action::operation;

  private:
    using operand_value = typename operand::value_type;
    using base = lazy_segment_tree_impl::base<operand, operation, action::map, action::fold>;

  public:
    using value_type = operand;
    using action_type = typename operation::value_type;

    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    explicit core(const size_type n = 0, const operand_value& v = {}) : base(n) {
        static_assert(action::tags.none() or action::tags.has(actions::flags::lazy_segment_tree));
        REP(p, 0, this->_n) this->_lengths[this->_size + p] = 1, this->_values[this->_size + p] = v;
        this->initialize();
    }

    template<class T> core(const std::initializer_list<T>& init_list) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) : base(std::distance(first, last)) {
        static_assert(action::tags.none() or action::tags.has(actions::flags::lazy_segment_tree));
        size_type p = 0;
        for(auto itr=first; itr!=last; ++itr, ++p) {
            this->_lengths[this->_size + p] = 1, this->_values[this->_size + p] = operand(*itr);
        }
        this->initialize();
    }

    bool empty() const { return this->size() == 0; }

    struct point_reference : internal::point_reference<core> {
        point_reference(core *const super, const size_type p)
          : internal::point_reference<core>(super, super->_positivize_index(p))
        {
            assert(0 <= this->_pos && this->_pos < this->size());
        }

        operator operand_value() const { return this->_super->get(this->_pos); }
        operand_value val() const { return this->_super->get(this->_pos); }

        inline point_reference& set(const operand_value& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const operand_value& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const action_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const action_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<core> {
        range_reference(core *const super, const size_type l, const size_type r)
          : internal::range_reference<core>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline range_reference& apply(const action_type& v) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator<<=(const action_type& v) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }

        inline value_type fold() {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }
        inline value_type operator*() {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto& set(size_type p, const operand_value& v) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        this->base::set(p, v);
         return *this;
    }

    inline auto& apply(size_type l, size_type r, const action_type& v) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->_super->size());
        this->base::apply(l, r, v);
        return *this;
    }
    inline auto& apply(const size_type p, const action_type& v) { this->apply(p, p+1, v); return *this; }
    inline auto& apply(const action_type& v) { this->apply(0, this->size(), v);  return *this; }
    inline auto& operator<<=(const action_type& v) { this->apply(0, this->size(), v);  return *this; }


    inline value_type get(size_type p) const {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        return this->base::get(p).val();
    }

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline value_type fold(size_type l, size_type r) const {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->_super->size());
        return this->base::fold(l, r).val();
    }
    inline value_type fold() const { return this->fold_all(); }


  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,core>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const core *const ref, const size_type p) : iterator_interface(ref, p) {}

        inline value_type operator*() const { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lazy_segment_tree_impl

} // namespace internal


template<class action> struct lazy_segment_tree : internal::lazy_segment_tree_impl::core<action> {
    using internal::lazy_segment_tree_impl::core<action>::core;
};


} // namespace lib
