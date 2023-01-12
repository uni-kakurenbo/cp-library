#pragma once


#include <vector>

#include "internal/dev_assert.hpp"
#include "data_structure/range_action/range_sum.hpp"


namespace lib {

namespace actions {

template<class T> struct range_add {};

} // namespace actions


template<class T> struct fenwick_tree<actions::range_add<T>> : fenwick_tree<actions::range_sum<T>> {
  private:
    using core = fenwick_tree<actions::range_sum<T>>;

  public:
    using value_type = typename core::value_type;
    using size_type = typename core::size_type;

    explicit fenwick_tree(const size_type size = 0) : core(size+1) {}
    explicit fenwick_tree(const size_type size, const value_type& val) : core(size+1, val) {}

    template<class U> fenwick_tree(const std::initializer_list<U>& init_list) : fenwick_tree(ALL(init_list)) {}

    template<class I, std::void_t<typename std::iterator_traits<I>::value_type>* = nullptr>
    fenwick_tree(const I first, const I last) : fenwick_tree(std::distance(first, last)) {
        size_type pos = 0;
        for(auto itr=first; itr!=last; ++itr, ++pos) this->apply(pos, *itr);
    }

    inline void add(const size_type first, const size_type last, const value_type& v) {
        this->core::add(first, v), this->core::apply(last, -v);
    }
    inline void add(const size_type p, const value_type& v) { this->apply(p, p+1, v); }
    inline void add(const value_type& v) { this->apply(0, this->size(), v); }

    inline void apply(const size_type first, const size_type last, const value_type& v) {
        this->core::add(first, v), this->core::apply(last, -v);
    }
    inline void apply(const size_type p, const value_type& v) { this->apply(p, p+1, v); }
    inline void apply(const value_type& v) { this->apply(0, this->size(), v); }

    inline void set(const size_type p, const value_type& v) {
        this->add(p, actions::range_sum<T>::rev(v, this->get(p)).val());
    }

    inline value_type get(const size_type p) const {
        dev_assert(0 <= p and p < this->size());
        return this->core::prod(p+1);
    }
    inline value_type operator[](const size_type pos) const { return this->get(pos); }


    struct iterator : virtual internal::container_iterator_interface<value_type,fenwick_tree> {
        iterator(const fenwick_tree *const ref, const size_type pos) : internal::container_iterator_interface<value_type,fenwick_tree>(ref, pos) {}

        inline value_type operator*() const override { return this->ref()->get(this->pos()); }
    };

    inline iterator begin() const { return iterator(this, 0); }
    inline iterator end() const { return iterator(this, this->size()); }
};


} // namespace lib
