#pragma once

#include <iterator>
#include <initializer_list>

#include <atcoder/fenwicktree>

#include "template.hpp"

#include "internal/develop/dev_assert.hpp"
#include "internal/iterator.hpp"
#include "internal/types.hpp"

namespace Lib {


template<class T> struct RangeSum : atcoder::fenwick_tree<T> {
  protected:
    Internal::Size _n;

  public:
    RangeSum(const Internal::Size n = 0) : atcoder::fenwick_tree<T>(n+1) { this->_n = n; }
    RangeSum(std::initializer_list<T> init_list) : RangeSum(ALL(init_list)) {}

    template<class I>
    RangeSum(const I first, const I last) : RangeSum(std::distance(first, last)) {
        for(auto itr=first; itr!=last; ++itr) this->set(itr-first, *itr);
    }

    inline Internal::Size size() const {
        return this->_n;
    }
    inline T get(const Internal::Size p) {
        dev_assert(0 <= p and p < this->size());
        return this->sum(p, p+1);
    }

    inline void set(const Internal::Size p, const T x) {
        dev_assert(0 <= p and p < this->size());
        this->add(p, x - this->get(p));
    }


    struct Iterator : virtual Internal::IContainerIterator<T,RangeSum> {
        using difference_type = typename Internal::IContainerIterator<T,RangeSum>::difference_type;

      public:
        Iterator(RangeSum *const ref, const Internal::Size pos) : Internal::IContainerIterator<T,RangeSum>(ref, pos) {}

        T operator*() const override { return this->ref()->get(this->pos()); }

        Iterator& operator++() override { return ++this->pos(), *this; }
        Iterator& operator--() override { return --this->pos(), *this; }

        Iterator& operator+=(const difference_type count) override { return this->pos() += count, *this; }
        Iterator& operator-=(const difference_type count) override { return this->pos() -= count, *this; }

    };
    using iterator = Iterator;

    Iterator begin() { return Iterator(this, 0); }
    Iterator end() { return Iterator(this, this->size()); }
};


} // namespace Lib
