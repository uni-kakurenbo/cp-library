#pragma once


#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <type_traits>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "numeric/bit.hpp"
#include "numeric/arithmetic.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace lazy_segment_tree_impl {

// Thanks to: atcoder::lazy_segtree
template<class S, class F, S (*_map)(const S&, const F&), F (*_fold)(const F&, const internal::size_t)>
struct base {
    using size_type = internal::size_t;

  public:
    size_type _n = 0, _size = 0, _depth = 0;
    mutable std::valarray<size_type> _lengths;
    mutable std::valarray<S> _values;
    mutable std::valarray<F> _lazy;

    inline void update(const size_type p) noexcept(NO_EXCEPT) {
        this->_values[p] = this->_values[p << 1] + this->_values[p << 1 | 1];
    }
    inline void all_apply(const size_type p, const F& f) const noexcept(NO_EXCEPT) {
        this->_values[p] = _map(this->_values[p], _fold(f, this->_lengths[p]));
        if(p < this->_size) this->_lazy[p] = f + this->_lazy[p];
    }
    inline void push(const size_type p) const noexcept(NO_EXCEPT) {
        this->all_apply(p << 1, this->_lazy[p]);
        this->all_apply(p << 1 | 1, this->_lazy[p]);
        this->_lazy[p] = F{};
    }

  protected:
    base() noexcept(NO_EXCEPT) {}

    explicit base(const size_type n) noexcept(NO_EXCEPT) : _n(n) {
        this->_size = lib::bit_ceil(lib::to_unsigned(n));
        this->_depth = lib::countr_zero(lib::to_unsigned(this->_size));
        this->_lengths.resize(2 * this->_size);
        this->_values.resize(2 * this->_size);
        this->_lazy.resize(this->_size);
    }

    inline void initialize() noexcept(NO_EXCEPT) {
        REPD(p, 1, this->_size) {
            this->_lengths[p] = this->_lengths[p << 1] + this->_lengths[p << 1 | 1];
            this->update(p);
        }
    }

  public:
    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type allocated() const noexcept(NO_EXCEPT) { return this->_size; }
    inline size_type depth() const noexcept(NO_EXCEPT) { return this->_depth; }

    inline void set(size_type p, const S& x) noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->push(p >> i);
        this->_values[p] = x;
        FOR(i, 1, this->_depth) this->update(p >> i);
    }

    inline S get(size_type p) const noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->push(p >> i);
        return this->_values[p];
    }

    inline S fold(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        if(l == r) return {};

        l += this->_size;
        r += this->_size;

        FORD(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->push(l >> i);
            if(((r >> i) << i) != r) this->push((r - 1) >> i);
        }

        S sml = S{}, smr = S{};
        while(l < r) {
            if(l & 1) sml = sml + this->_values[l++];
            if(r & 1) smr = this->_values[--r] + smr;
            l >>= 1;
            r >>= 1;
        }

        return sml + smr;
    }

    inline S fold_all() const noexcept(NO_EXCEPT) { return this->_values[1]; }

    inline void apply(size_type p, const F& f) noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->push(p >> i);
        this->_values[p] = _map(this->_values[p], _fold(f, this->_lengths[p]));
        FOR(i, 1, this->_depth) this->update(p >> i);
    }
    inline void apply(size_type l, size_type r, const F& f) noexcept(NO_EXCEPT) {
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

    template<bool (*g)(S)> inline size_type max_right(size_type l) const noexcept(NO_EXCEPT) {
        return this->max_right(l, [](S x) { return g(x); });
    }
    template<class G> inline size_type max_right(size_type l, G g) const noexcept(NO_EXCEPT) {
        assert(0 <= l && l <= _n);
        assert(g({}));
        if(l == _n) return _n;
        l += this->_size;
        FORD(i, 1, this->_depth) this->push(l >> i);
        S sm = S{};
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

    template<bool (*g)(S)> inline size_type min_left(size_type r) const noexcept(NO_EXCEPT) {
        return min_left(r, [](S x) { return g(x); });
    }
    template<class G> inline size_type min_left(size_type r, G g) const noexcept(NO_EXCEPT) {
        assert(0 <= r && r <= _n);
        assert(g({}));
        if(r == 0) return 0;
        r += this->_size;
        FORD(i, 1, this->_depth) this->push((r - 1) >> i);
        S sm = S{};
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
    using base = lazy_segment_tree_impl::base<operand, operation, action::map, action::fold>;

  public:
    using value_type = operand;
    using action_type = typename operation::value_type;

    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    core() noexcept(NO_EXCEPT) : base() {}
    explicit core(const size_type n, const value_type& v = {}) noexcept(NO_EXCEPT) : base(n) { this->fill(v); }

    template<class T> core(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) noexcept(NO_EXCEPT) : base(static_cast<size_type>(std::distance(first, last))) { this->assign(first, last); }

    template<class T>
    inline auto& assign(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT) { return this->assign(ALL(init_list)); }

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    inline auto& assign(const I first, const I last) noexcept(NO_EXCEPT) {
        assert(std::distance(first, last) == this->_n);
        size_type p = 0;
        for(auto itr=first; itr!=last; ++itr, ++p) {
            this->_lengths[this->_size + p] = 1, this->_values[this->_size + p] = value_type(*itr);
        }
        this->initialize();
        return *this;
    }

    inline auto& fill( const value_type& v = {}) noexcept(NO_EXCEPT) {
        REP(p, 0, this->_n) {
            this->_lengths[this->_size + p] = 1, this->_values[this->_size + p] = v;
        }
        this->initialize();
        return *this;
    }

    bool empty() const noexcept(NO_EXCEPT) { return this->size() == 0; }

    struct point_reference : internal::point_reference<core> {
        point_reference(core *const super, const size_type p) noexcept(NO_EXCEPT)
          : internal::point_reference<core>(super, super->_positivize_index(p))
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

        inline point_reference& apply(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<core> {
        range_reference(core *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<core>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline range_reference& apply(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }
        inline range_reference& operator<<=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }

        inline value_type fold() noexcept(NO_EXCEPT) {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }
        inline value_type operator*() noexcept(NO_EXCEPT) {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto& set(size_type p, const value_type& v) noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        this->base::set(p, v);
         return *this;
    }

    inline auto& apply(size_type l, size_type r, const action_type& v) noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        this->base::apply(l, r, v);
        return *this;
    }
    inline auto& apply(const size_type p, const action_type& v) noexcept(NO_EXCEPT) { this->apply(p, p+1, v); return *this; }
    inline auto& apply(const action_type& v) noexcept(NO_EXCEPT) { this->apply(0, this->size(), v);  return *this; }


    inline value_type get(size_type p) const noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->size());
        return this->base::get(p).val();
    }

    inline point_reference operator[](const size_type p) noexcept(NO_EXCEPT) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }

    inline value_type fold(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->size());
        return this->base::fold(l, r).val();
    }
    inline value_type fold() const noexcept(NO_EXCEPT) { return this->fold_all(); }

    struct iterator : internal::container_iterator_interface<value_type,core,iterator> {
        iterator() noexcept = default;
        iterator(const core *const ref, const size_type p) noexcept(NO_EXCEPT) : internal::container_iterator_interface<value_type,core,iterator>(ref, p) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace lazy_segment_tree_impl

} // namespace internal


template<class action> struct lazy_segment_tree : internal::lazy_segment_tree_impl::core<action> {
    using internal::lazy_segment_tree_impl::core<action>::core;
};


} // namespace lib
