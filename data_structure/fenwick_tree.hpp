#pragma once

#include <vector>
#include <iterator>
#include <utility>

#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/dev_assert.hpp"
#include "internal/point_reference.hpp"
#include "internal/range_reference.hpp"

#include "snippet/iterations.hpp"

#include "data_structure/range_action/flags.hpp"


namespace lib {

namespace internal {

namespace fenwick_tree_lib {


// Thanks to: atcoder::fenwick_tree
template<class S,S (*r_op)(const S&,const S&)>
struct base {
    using size_type = internal::size_t;

  private:
    size_t _n;
    S *const _data;

  protected:
    S fold(size_type r) const {
        S s = S{0};
        while (r > 0) {
            s = s * _data[r-1];
            r -= r & -r;
        }
        return s;
    }

  protected:
    explicit base(const size_type n = 0) : _n(n), _data(new S[n]()) {}

    ~base() { delete[] this->_data; }

  public:
    inline size_type size() const { return this->_n; }

    inline void apply(size_type p, const S& x) {
        p++;
        while (p <= _n) {
            _data[p-1] = _data[p-1] * x;
            p += p & -p;
        }
    }

    inline void set(const size_type p, const S& x) {
        this->apply(p, r_op(x, this->fold(p, p+1)));
    }

    inline S fold(const size_type l, const size_type r) const {
        return r_op(this->fold(r), this->fold(l));
    }
};


template<class Action>
struct core : base<typename Action::operand,Action::rev> {
  public:
    using action = Action;

    using operand = typename action::operand;

  private:
    using base = typename fenwick_tree_lib::base<operand,action::rev>;

  public:
    using value_type = typename operand::value_type;
    using size_type = typename base::size_type;

  protected:
    inline void _validate_index_in_right_open([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p < this->size());
    }
    inline void _validate_index_in_closed([[maybe_unused]] const size_type p) const {
        dev_assert(0 <= p and p <= this->size());
    }
    inline void _validate_rigth_open_interval([[maybe_unused]] const size_type l, [[maybe_unused]] const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
    }

    inline size_type _positivize_index(const size_type p) const {
        return p < 0 ? this->size() + p : p;
    }

  public:
    explicit core(const size_type n = 0) : base(n) {
        static_assert(action::tags.bits() == 0 or action::tags.has(actions::flags::fenwick_tree));
    }
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
            this->_super->_validate_index_in_right_open(this->_pos);
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
            this->_super->_validate_rigth_open_interval(this->_begin, this->_end);
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


    inline auto& apply(const size_type p, const value_type& x) {
        dev_assert(0 <= p and p < this->size());
        this->base::apply(p, x);
         return *this;
    }

    inline auto& set(const size_type p, const value_type& x) {
        dev_assert(0 <= p and p < this->size());
        this->base::set(p, x);
         return *this;
    }

    inline value_type get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->base::fold(p, p+1);
    }

    inline point_reference operator[](const size_type p) { return point_reference(this, p); }

    inline const range_reference operator()(const size_type l, const size_type r) const { return range_reference(this, l, r); }
    inline range_reference operator()(const size_type l, const size_type r) { return range_reference(this, l, r); }

    inline value_type fold(const size_type l, const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
        return this->base::fold(l, r);
    }
    inline value_type fold(const size_type r) const {
        dev_assert(0 <= r and r <= this->size());
        return this->base::fold(0, r);
    }
    inline value_type fold() const {
        return this->base::fold(0, this->size());
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


} // namespace fenwick_tree_lib

} // namespace internal


template<class action> struct fenwick_tree : internal::fenwick_tree_lib::core<action> {
    using internal::fenwick_tree_lib::core<action>::core;
};


} // namespace lib
