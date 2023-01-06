#pragma once


#include "internal/types.hpp"
#include "internal/iterator.hpp"
#include "internal/dev_assert.hpp"

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
    S prod(size_type r) const {
        S s = 0;
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
        this->apply(p, r_op(x, this->prod(p, p+1)));
    }

    inline S prod(const size_type l, const size_type r) const {
        return r_op(this->prod(r), this->prod(l));
    }

};


template<class Action>
struct core : base<typename Action::operand_monoid,Action::rev> {
  public:
    using action = Action;

    using operand_monoid = typename action::operand_monoid;

  private:
    using base = typename fenwick_tree_lib::base<operand_monoid,action::rev>;

  public:
    using value_type = typename operand_monoid::value_type;
    using size_type = typename base::size_type;

  public:
    explicit core(const size_type n = 0) : base(n) {
        static_assert(action::tags.bits() == 0 or action::tags.has(actions::flags::fenwick_tree));
    }
    explicit core(const size_type n, const value_type& v) : base(n) { REP(i, n) this->base::apply(i, v); }
    explicit core(const std::initializer_list<value_type>& init_list) : core(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    explicit core(const I first, const I last) : core(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->base::apply(pos, *itr);
    }

    void apply(const size_type pos, const value_type& x) {
        dev_assert(0 <= pos and pos < this->size());
        this->base::apply(pos, x);
    }

    inline void set(const size_type pos, const value_type& x) {
        dev_assert(0 <= pos and pos < this->size());
        this->base::set(pos, x);
    }

    inline value_type get(const size_type pos) const {
        dev_assert(0 <= pos and pos < this->size());
        return this->base::prod(pos, pos+1).val();
    }
    inline value_type operator[](size_type pos) const {
        dev_assert(0 <= pos and pos < this->size());
        return this->base::get(pos);
    }

    inline value_type prod(const size_type l, const size_type r) const {
        dev_assert(0 <= l and l <= r and r <= this->size());
        return this->base::prod(l, r).val();
    }
    inline value_type prod(const size_type r) const {
        dev_assert(0 <= r and r <= this->size());
        return this->base::prod(0, r).val();
    }
    inline value_type prod() const {
        return this->base::prod(0, this->size()).val();
    }


    struct iterator : virtual internal::container_iterator_interface<value_type,core> {
        iterator(const core *const ref, const size_type pos) : internal::container_iterator_interface<value_type,core>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
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
