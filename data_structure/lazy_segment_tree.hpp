#pragma once


#include <cassert>
#include <algorithm>
#include <iostream>
#include <vector>
#include <type_traits>
#include <concepts>
#include <ranges>
#include <bit>


#include "snippet/aliases.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "numeric/bit.hpp"
#include "numeric/arithmetic.hpp"

#include "algebraic/internal/concepts.hpp"

#include "action/base.hpp"
#include "action/helpers.hpp"


namespace uni {

namespace internal {

namespace lazy_segment_tree_impl {

// Thanks to: atcoder::lazy_segtree
template<actions::internal::full_action Action>
    requires
        algebraic::internal::monoid<typename Action::operand> &&
        algebraic::internal::monoid<typename Action::operation>
struct core {
    using size_type = internal::size_t;

    using action = Action;
    using operand = typename Action::operand;
    using operation = typename Action::operation;

 private:
    size_type _n = 0, _size = 0, _depth = 0;
    std::valarray<size_type> _lengths;
    std::valarray<operand> _values;
    std::valarray<operation> _lazy;


    inline void _pull(const size_type p) noexcept(NO_EXCEPT) {
        this->_values[p] = this->_values[p << 1] + this->_values[p << 1 | 1];
    }

    inline void _all_apply(const size_type p, const operation& f) noexcept(NO_EXCEPT) {
        this->_values[p] = action::map(this->_values[p], action::fold(f, this->_lengths[p]));
        if(p < this->_size) this->_lazy[p] = f + this->_lazy[p];
    }

    inline void _push(const size_type p) noexcept(NO_EXCEPT) {
        this->_all_apply(p << 1, this->_lazy[p]);
        this->_all_apply(p << 1 | 1, this->_lazy[p]);
        this->_lazy[p] = operation{};
    }

    inline void _init() noexcept(NO_EXCEPT) {
        REPD(p, 1, this->_size) {
            this->_lengths[p] = this->_lengths[p << 1] + this->_lengths[p << 1 | 1];
            this->_pull(p);
        }
    }

  public:
    core() noexcept = default;

    explicit core(const size_type n) noexcept(NO_EXCEPT)
      : _n(n), _size(std::bit_ceil(uni::to_unsigned(n))), _depth(std::countr_zero(uni::to_unsigned(this->_size))),
        _lengths(this->_size << 1), _values(this->_size << 1), _lazy(this->_size)
    {}


    inline size_type size() const noexcept(NO_EXCEPT) { return this->_n; }
    inline size_type allocated() const noexcept(NO_EXCEPT) { return this->_values.size(); }
    inline size_type depth() const noexcept(NO_EXCEPT) { return this->_depth; }


    inline operand fold_all() const noexcept(NO_EXCEPT) { return this->_values[1]; }


    inline void fill( const operand& v = {}) noexcept(NO_EXCEPT) {
        REP(p, 0, this->_n) {
            this->_lengths[this->_size + p] = 1, this->_values[this->_size + p] = v;
        }
        this->_init();
    }

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline void assign(I first, S last) noexcept(NO_EXCEPT) {
        if constexpr(std::sized_sentinel_for<operand, I>) {
            assert(std::ranges::distance(first, last) == this->_n);
        }
        size_type p = 0;
        for(auto itr=first; itr!=last; ++itr, ++p) {
            this->_lengths[this->_size + p] = 1, this->_values[this->_size + p] = static_cast<operand>(*itr);
        }
        this->_init();
    }


    inline void set(size_type p, const operand& x) noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->_push(p >> i);
        this->_values[p] = x;
        FOR(i, 1, this->_depth) this->_pull(p >> i);
    }

    inline void add(size_type p, const operand& x) noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->_push(p >> i);
        this->_values[p] = this->_values[p] + x;
        FOR(i, 1, this->_depth) this->_pull(p >> i);
    }

    inline operand get(size_type p) noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->_push(p >> i);
        return this->_values[p];
    }

    inline operand fold(size_type l, size_type r) noexcept(NO_EXCEPT) {
        if(l == r) return {};

        l += this->_size;
        r += this->_size;

        FORD(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->_push(l >> i);
            if(((r >> i) << i) != r) this->_push((r - 1) >> i);
        }

        operand sml = operand{}, smr = operand{};
        while(l < r) {
            if(l & 1) sml = sml + this->_values[l++];
            if(r & 1) smr = this->_values[--r] + smr;
            l >>= 1;
            r >>= 1;
        }

        return sml + smr;
    }


    inline void apply(size_type p, const operation& f) noexcept(NO_EXCEPT) {
        p += this->_size;
        FORD(i, 1, this->_depth) this->_push(p >> i);
        this->_values[p] = action::map(this->_values[p], action::fold(f, this->_lengths[p]));
        FOR(i, 1, this->_depth) this->_pull(p >> i);
    }

    inline void apply(size_type l, size_type r, const operation& f) noexcept(NO_EXCEPT) {
        if(l == r) return;

        l += this->_size;
        r += this->_size;

        FORD(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->_push(l >> i);
            if(((r >> i) << i) != r) this->_push((r - 1) >> i);
        }

        {
            size_type l2 = l, r2 = r;
            while(l < r) {
                if(l & 1) this->_all_apply(l++, f);
                if(r & 1) this->_all_apply(--r, f);
                l >>= 1;
                r >>= 1;
            }
            l = l2;
            r = r2;
        }

        FOR(i, 1, this->_depth) {
            if(((l >> i) << i) != l) this->_pull(l >> i);
            if(((r >> i) << i) != r) this->_pull((r - 1) >> i);
        }
    }


    template<class F>
    inline size_type max_right(size_type l, F&& f) noexcept(NO_EXCEPT) {
        assert(0 <= l && l <= _n);
        assert(f(operand{}));
        if(l == _n) return _n;
        l += this->_size;
        FORD(i, 1, this->_depth) this->_push(l >> i);
        operand sm;
        do {
            while(l % 2 == 0) l >>= 1;
            if(!f(sm + this->_values[l])) {
                while(l < this->_size) {
                    this->_push(l);
                    l = (2 * l);
                    if(f(sm + this->_values[l])) {
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


    template<class F>
    inline size_type min_left(size_type r, F&& f) noexcept(NO_EXCEPT) {
        assert(0 <= r && r <= _n);
        assert(f(operand{}));
        if(r == 0) return 0;
        r += this->_size;
        FORD(i, 1, this->_depth) this->_push((r - 1) >> i);
        operand sm;
        do {
            r--;
            while(r > 1 && (r % 2)) r >>= 1;
            if(!f(this->_values[r] + sm)) {
                while(r < this->_size) {
                    this->_push(r);
                    r = (2 * r + 1);
                    if(f(this->_values[r] + sm)) {
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


} // namespace lazy_segment_tree_impl

} // namespace internal


template<class T>
struct lazy_segment_tree : lazy_segment_tree<actions::make_full_t<T>> {
    using lazy_segment_tree<actions::make_full_t<T>>::lazy_segment_tree;
};


template<actions::internal::full_action Action>
    requires internal::available<internal::lazy_segment_tree_impl::core<Action>>
struct lazy_segment_tree<Action> {
    using action = Action;
    using operand = typename Action::operand;
    using operation = typename Action::operation;

  private:
    using core = internal::lazy_segment_tree_impl::core<action>;

    core _impl;

  public:
    using value_type = operand;
    using action_type = typename operation::value_type;

    using size_type = typename core::size_type;

    inline auto size() const noexcept(NO_EXCEPT) { return this->_impl.size(); }
    inline auto allocated() const noexcept(NO_EXCEPT) { return this->_impl.allocated(); }
    inline auto depth() const noexcept(NO_EXCEPT) { return this->_impl.depth(); }

  protected:
    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->_impl.size() + p : p;
    }

  public:
    lazy_segment_tree() noexcept(NO_EXCEPT) : _impl() {}

    explicit lazy_segment_tree(const size_type n, const value_type& v = {}) noexcept(NO_EXCEPT) : _impl(n) { this->_impl.fill(v); }

    template<std::convertible_to<value_type> T>
    lazy_segment_tree(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT) : lazy_segment_tree(ALL(init_list)) {}

    template<std::input_iterator I, std::sized_sentinel_for<I> S>
    lazy_segment_tree(I first, S last) noexcept(NO_EXCEPT)
      : _impl(static_cast<size_type>(std::ranges::distance(first, last)))
    { this->assign(first, last); }

    template<std::ranges::input_range R>
    explicit lazy_segment_tree(R&& range) noexcept(NO_EXCEPT) : lazy_segment_tree(ALL(range)) {}

    template<std::convertible_to<value_type> T>
    inline auto& assign(const std::initializer_list<T>& init_list) noexcept(NO_EXCEPT)
    {
        return this->assign(ALL(init_list));
    }

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline auto& assign(I first, S last) noexcept(NO_EXCEPT) {
        this->_impl.assign(first, last);
        return *this;
    }

    template<std::ranges::input_range R>
    inline auto& assign(R&& range) noexcept(NO_EXCEPT) { return this->assign(ALL(range)); }

    inline auto& fill( const value_type& v = {}) noexcept(NO_EXCEPT) {
        this->impl.fill(v);
        return *this;
    }

    bool empty() const noexcept(NO_EXCEPT) { return this->_impl.size() == 0; }

    struct point_reference : internal::point_reference<lazy_segment_tree> {
        point_reference(lazy_segment_tree *const super, const size_type p) noexcept(NO_EXCEPT)
          : internal::point_reference<lazy_segment_tree>(super, super->_positivize_index(p))
        {
            assert(0 <= this->_pos && this->_pos < this->_super->size());
        }

        operator value_type() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }
        auto val() noexcept(NO_EXCEPT) { return this->_super->get(this->_pos); }

        inline auto& operator=(const value_type& v) noexcept(NO_EXCEPT) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline auto& operator+=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->add(this->_pos, v);
            return *this;
        }

        inline auto& operator*=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
    };

    struct range_reference : internal::range_reference<lazy_segment_tree> {
        range_reference(lazy_segment_tree *const super, const size_type l, const size_type r) noexcept(NO_EXCEPT)
          : internal::range_reference<lazy_segment_tree>(super, super->_positivize_index(l), super->_positivize_index(r))
        {
            assert(0 <= this->_begin && this->_begin <= this->_end && this->_end <= this->_super->size());
        }

        inline auto& operator*=(const action_type& v) noexcept(NO_EXCEPT) {
            this->_super->apply(this->_begin, this->_end, v);
            return *this;
        }

        inline auto fold() noexcept(NO_EXCEPT) {
            if(this->_begin == 0 && this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto& set(size_type p, const value_type& v) noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->_impl.size());
        this->_impl.set(p, v);
         return *this;
    }

    inline auto& add(size_type p, const value_type& v) noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->_impl.size());
        this->_impl.add(p, v);
         return *this;
    }


    inline auto& apply(size_type l, size_type r, const action_type& v) noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->_impl.size());
        this->_impl.apply(l, r, v);
        return *this;
    }

    inline auto& apply(const size_type p, const action_type& v) noexcept(NO_EXCEPT) {
        this->apply(p, p + 1, v);
        return *this;
    }

    inline auto& apply(const action_type& v) noexcept(NO_EXCEPT) { this->apply(0, this->_impl.size(), v);  return *this; }


    inline auto get(size_type p) noexcept(NO_EXCEPT) {
        p = this->_positivize_index(p), assert(0 <= p && p < this->_impl.size());
        return this->_impl.get(p);
    }

    inline auto operator[](const size_type p) noexcept(NO_EXCEPT) { return point_reference(this, p); }
    inline auto operator()(const size_type l, const size_type r) noexcept(NO_EXCEPT) { return range_reference(this, l, r); }


    inline auto fold(size_type l, size_type r) noexcept(NO_EXCEPT) {
        l = this->_positivize_index(l), r = this->_positivize_index(r);
        assert(0 <= l && l <= r && r <= this->_impl.size());
        return this->_impl.fold(l, r);
    }
    inline auto fold() noexcept(NO_EXCEPT) { return this->_impl.fold_all(); }



    template<bool (*f)(value_type)>
    inline auto max_right(const size_type l) noexcept(NO_EXCEPT) {
        return this->max_right(l, [](operand x) { return f(x); });
    }

    template<class F>
    inline auto max_right(const size_type l, F&& f) noexcept(NO_EXCEPT) {
        return this->_impl.max_right(l, std::forward<F>(f));
    }


    template<bool (*f)(value_type)>
    inline auto min_left(const size_type r) noexcept(NO_EXCEPT) {
        return min_left(r, [](operand x) { return f(x); });
    }

    template<class F>
    inline auto min_left(const size_type r, F&& f) noexcept(NO_EXCEPT) {
        return this->_impl.min_left(r, std::forward<F>(f));
    }


    struct iterator : internal::container_iterator_interface<value_type, lazy_segment_tree, iterator> {
        iterator() noexcept = default;

        iterator(lazy_segment_tree *const ref, const size_type p) noexcept(NO_EXCEPT)
          : internal::container_iterator_interface<value_type, lazy_segment_tree, iterator>(ref, p)
        {}

        inline auto operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    };

    inline auto begin() noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline auto end() noexcept(NO_EXCEPT) { return iterator(this, this->_impl.size()); }
};


} // namespace uni
