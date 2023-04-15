#pragma once


#include <cassert>
#include <algorithm>
#include <vector>

#include <type_traits>

#include <atcoder/internal_bit>

#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"
#include "internal/uncopyable.hpp"

#include "snippet/iterations.hpp"

#include "numeric/bit.hpp"

#include "data_structure/range_action/flags.hpp"
#include "data_structure/internal/is_action.hpp"
#include "algebraic/internal/traits.hpp"


namespace lib {

namespace internal {

namespace segment_tree_impl {


// Thanks to: atcoder::segtree
template<class S> struct base : private lib::internal::uncopyable {
    using size_type = internal::size_t;

  protected:
    size_type _n = 0, _size = 0, _depth = 0;
    S* _data = nullptr;

    inline void update(const size_type k) { this->_data[k] = this->_data[k << 1] + this->_data[k << 1 | 1]; }

  protected:
    base() {}
    explicit base(const size_type n) : _n(n), _depth(bit_width<unsigned>(n - 1)) {
        this->_size = 1 << this->_depth;
        this->_data = new S[this->_size << 1]();
    }
    ~base() { delete[] this->_data; }

  public:
    inline size_type size() const { return this->_n; }
    inline size_type allocated() const { return this->_size; }
    inline size_type depth() const { return this->_depth; }

  protected:
    inline void apply(size_type p, const S& x) {
        this->set(p, this->_data[p + this->_size] + x);
    }

    inline void set(size_type p, const S& x) {
        p += this->_size;
        this->_data[p] = x;
        FOR(i, 1, this->_depth) this->update(p >> i);
    }

    inline S get(size_type p) const {
        return this->_data[p + this->_size];
    }

    inline S fold(size_type l, size_type r) const {
        S sml, smr;
        l += this->_size;
        r += this->_size;

        while(l < r) {
            if(l & 1) sml = sml + this->_data[l++];
            if(r & 1) smr = this->_data[--r] + smr;
            l >>= 1;
            r >>= 1;
        }
        return sml + smr;
    }

    inline S fold_all() const { return this->_data[1]; }

  public:
    template<bool (*f)(S)> inline size_type max_right(const size_type l) const {
        return this->max_right(l, [](S x) { return f(x); });
    }
    template<class F> inline size_type max_right(size_type l, const F& f) const {
        assert(0 <= l && l <= this->_n);
        assert(f(S{}));
        if(l == this->_n) return this->_n;
        l += this->_size;
        S sm;
        do {
            while(l%2 == 0) l >>= 1;
            if(!f(sm + this->_data[l])) {
                while(l < this->_size) {
                    l <<= 1;
                    if(f(sm + this->_data[l])) {
                        sm = sm + this->_data[l];
                        ++l;
                    }
                }
                return l - this->_size;
            }
            sm = sm + this->_data[l];
            ++l;
        } while((l & -l) != l);
        return this->_n;
    }

    template<bool (*f)(S)> inline size_type min_left(const size_type r) const {
        return this->min_left(r, [](S x) { return f(x); });
    }
    template<class F> inline size_type min_left(size_type r, const F& f) const {
        assert(0 <= r && r <= this->_n);
        assert(f(S()));
        if (r == 0) return 0;
        r += this->_size;
        S sm;
        do {
            --r;
            while(r > 1 and (r%2)) r >>= 1;
            if(!f(this->_data[r] + sm)) {
                while(r < this->_size) {
                    r = (r << 1 | 1);
                    if(f(this->_data[r] + sm)) {
                        sm = this->_data[r] + sm;
                        --r;
                    }
                }
                return r + 1 - this->_size;
            }
            sm = this->_data[r] + sm;
        } while((r & -r) != r);
        return 0;
    }
};


template<class, class = std::void_t<>> struct core {};

template<class monoid>
struct core<monoid, std::void_t<typename algebraic::internal::is_monoid_t<monoid>>> : base<monoid> {
  private:
    using monoid_value = typename monoid::value_type;
    using base = typename segment_tree_impl::base<monoid>;

  public:
    using value_type = monoid;
    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    core() : base() {}
    explicit core(const size_type n, const monoid_value& v = {}) : base(n) { this->fill(v); }

    template<class T>
    core(const std::initializer_list<T>& init_list) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) : core(std::distance(first, last)) { this->assign(first, last); }

    template<class T>
    inline auto& assign(const std::initializer_list<T>& init_list) { return this->assign(ALL(init_list)); }

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    inline auto& assign(const I first, const I last) {
        assert(std::distance(first, last) == this->size());
        size_type p = 0;
        for(auto itr=first; itr!=last; ++itr, ++p) this->_data[this->_size + p] = monoid(*itr);
        REPD(p, 1, this->_size) this->update(p);
        return *this;
    }

    inline auto& fill(const monoid_value& v = {}) {
        REP(p, this->_n) this->_data[this->_size + p] = v;
        REPD(p, 1, this->_size) this->update(p);
        return *this;
    }

    bool empty() const { return this->size() == 0; }

    struct point_reference : internal::point_reference<core> {
        point_reference(core *const super, const size_type p)
          : internal::point_reference<core>(super, super->_positivize_index(p))
        {
            assert(0 <= this->_pos && this->_pos < this->_super->size());
        }

        operator monoid_value() const { return this->_super->get(this->_pos); }
        monoid_value val() const { return this->_super->get(this->_pos); }

        inline point_reference& set(const monoid_value& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const monoid_value& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const monoid_value& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const monoid_value& v) {
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

        inline value_type fold() {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            return this->_super->fold(this->_begin, this->_end);
        }
        inline value_type operator*() {
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto& apply(const size_type p, const monoid_value& x) {
        assert(0 <= p && p < this->size());
        this->base::apply(p, x);
         return *this;
    }

    inline auto& set(const size_type p, const monoid_value& x) {
        assert(0 <= p && p < this->size());
        this->base::set(p, x);
         return *this;
    }

    inline value_type get(const size_type p) const {
        assert(0 <= p && p < this->size());
        return this->base::fold(p, p+1);
    }

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline value_type fold(const size_type l, const size_type r) const {
        assert(0 <= l && l <= r && r <= this->size());
        return this->base::fold(l, r);
    }
    inline value_type fold(const size_type r) const {
        assert(0 <= r && r <= this->size());
        return this->base::fold(0, r);
    }
    inline value_type fold() const {
        return this->base::fold_all();
    }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_type p) : internal::container_iterator_interface<value_type,core>(ref, p) {}

        inline value_type operator*() const { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};

template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand> {
    using action = Action;
    using core<typename action::operand>::core;

    static_assert(Action::tags.none() or Action::tags.has(actions::flags::range_folding));
};


} // namespace segment_tree_impl

} // namespace internal


template<class monoid> struct segment_tree : internal::segment_tree_impl::core<monoid> {
    using internal::segment_tree_impl::core<monoid>::core;
};


} // namespace lib
