#pragma once

#include <cassert>
#include <vector>
#include <iterator>
#include <utility>

#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "snippet/iterations.hpp"

#include "data_structure/range_action/flags.hpp"
#include "data_structure/internal/is_action.hpp"
#include "algebraic/internal/is_algebraic.hpp"
#include "algebraic/internal/is_commutative.hpp"


namespace lib {

namespace internal {

namespace fenwick_tree_impl {


// Thanks to: atcoder::fenwick_tree
template<class S>
struct base {
    using size_type = internal::size_t;

  private:
    size_t _n;
    S *const _data;

  protected:
    explicit base(const size_type n = 0) : _n(n), _data(new S[n]()) {}

    ~base() { delete[] this->_data; }

  public:
    inline size_type size() const { return this->_n; }

    inline void apply(size_type p, const S& x) {
        p++;
        while (p <= _n) {
            _data[p-1] = _data[p-1] + x;
            p += p & -p;
        }
    }

    inline void set(const size_type p, const S& x) {
        this->apply(p, x + -this->fold(p, p+1));
    }

    inline S fold(size_type r) const {
        S s = S{0};
        while (r > 0) {
            s = s + _data[r-1];
            r -= r & -r;
        }
        return s;
    }

    inline S fold(const size_type l, const size_type r) const {
        return this->fold(r) + -this->fold(l);
    }
};


template<class, class = std::void_t<>> struct core {};

template<class monoid>
struct core<monoid,std::void_t<typename algebraic::internal::is_monoid_t<monoid>>> : base<monoid> {
    static_assert(algebraic::internal::is_commutative_v<monoid>, "commutative property is required");

  private:
    using base = typename fenwick_tree_impl::base<monoid>;

  public:
    using value_type = monoid;
    using size_type = typename base::size_type;

  protected:
    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    explicit core(const size_type n = 0) : base(n) {}
    explicit core(const size_type n, const value_type& v) : base(n) { REP(i, n) this->base::apply(i, v); }
    template<class T> core(const std::initializer_list<T>& init_list) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) : core(std::distance(first, last)) {
        size_type p = 0;
        for(auto itr=first; itr!=last; ++itr, ++p) this->base::apply(p, *itr);
    }

    bool empty() const { return this->size() == 0; }

    struct point_reference : internal::point_reference<core> {
        point_reference(core *const super, const size_type p)
          : internal::point_reference<core>(super, super->_positivize_index(p))
        {
            assert(0 <= this->_pos && this->_pos < this->size());
        }

        operator value_type() const { return this->_super->get(this->_pos); }
        value_type val() const { return this->_super->get(this->_pos); }

        inline point_reference& set(const value_type& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }
        inline point_reference& operator=(const value_type& v) {
            this->_super->set(this->_pos, v);
            return *this;
        }

        inline point_reference& apply(const value_type& v) {
            this->_super->apply(this->_pos, v);
            return *this;
        }
        inline point_reference& operator<<=(const value_type& v) {
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
            if(this->_begin == 0) return this->_super->fold(this->_end);
            return this->_super->fold(this->_begin, this->_end);
        }
        inline value_type operator*() {
            if(this->_begin == 0 and this->_end == this->_super->size()) return this->_super->fold();
            if(this->_begin == 0) return this->_super->fold(this->_end);
            return this->_super->fold(this->_begin, this->_end);
        }
    };


    inline auto& apply(const size_type p, const value_type& x) {
        assert(0 <= p && p < this->size());
        this->base::apply(p, x);
         return *this;
    }

    inline auto& set(const size_type p, const value_type& x) {
        static_assert(algebraic::internal::is_group_v<value_type>, "point setting requires inverse element");
        assert(0 <= p && p < this->size());
        this->base::set(p, x);
         return *this;
    }

    inline value_type get(const size_type p) const {
        static_assert(algebraic::internal::is_group_v<value_type>, "point getting requires inverse element");
        assert(0 <= p && p < this->size());
        return this->base::fold(p, p+1);
    }

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }

    inline const range_reference operator()(const size_type l, const size_type r) const { return range_reference(this, l, r); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline value_type fold(const size_type l, const size_type r) const {
        static_assert(algebraic::internal::is_group_v<value_type>, "range folding requires inverse element");
        assert(0 <= l && l <= r && r <= this->size());
        return this->base::fold(l, r);
    }
    inline value_type fold(const size_type r) const {
        assert(0 <= r && r <= this->size());
        return this->base::fold(r);
    }
    inline value_type fold() const {
        return this->base::fold(this->size());
    }


  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,core>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const core *const ref, const size_type p) : iterator_interface(ref, p) {}

        inline value_type operator*() const { return this->ref()->get(this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const { return *(*this + count); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};

template<class Action>
struct core<Action, std::void_t<typename internal::is_action_t<Action>>> : core<typename Action::operand> {
    using action = Action;
    using core<typename action::operand>::core;

    static_assert(action::tags.none() or action::tags.has(actions::flags::range_folding));
};


} // namespace fenwick_tree_impl

} // namespace internal


template<class action> struct fenwick_tree : internal::fenwick_tree_impl::core<action> {
    using internal::fenwick_tree_impl::core<action>::core;
};


} // namespace lib
