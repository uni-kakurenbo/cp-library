#pragma once


#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "internal/dev_assert.hpp"

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
    std::vector<S> data;

  protected:
    S prod(size_type r) const {
        S s = 0;
        while (r > 0) {
            s = s * data[r - 1];
            r -= r & -r;
        }
        return s;
    }

  public:
    explicit base(const size_type n = 0) : _n(n), data(n) {}

    inline size_type size() const { return this->_n; }

    inline void apply(size_type p, const S& x) {
        p++;
        while (p <= _n) {
            data[p-1] = data[p-1] * x;
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
        static_assert(action::tags.has(actions::flags::fenwick_tree));
    }

    explicit core(const size_type n, const value_type& v) : base(n) { if(v != 0) REP(i, n) this->base::apply(i, v); }

    core(const std::initializer_list<value_type>& init_list) : core(ALL(init_list)) {}

    template<class I, std::enable_if_t<std::is_same_v<value_type, typename std::iterator_traits<I>::value_type>>* = nullptr>
    core(const I first, const I last) : core(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->base::apply(pos, *itr);
    }

    void apply(const size_type p, const value_type& x) {
        dev_assert(0 <= p && p < this->size());
        this->base::apply(p, x);
    }

    inline void set(const size_type p, const value_type& x) {
        this->base::set(p, x);
    }

    inline value_type get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->base::prod(p, p+1).val();
    }
    inline value_type operator[](size_type pos) const { return this->get(pos); }

    inline value_type prod(const size_type l, const size_type r) const {
        dev_assert(0 <= l && l <= r && r <= this->size());
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
