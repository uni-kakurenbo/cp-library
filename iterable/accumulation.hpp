#pragma once

#include <cassert>
#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

#include "internal/types.hpp"

#include "valarray.hpp"

namespace lib {


template<class T = i64, class container = valarray<T>>
struct accumulation : container {
    using size_type = internal::size_t;

  protected:
    inline size_type _positivize_index(const size_type x) const {
        return x < 0 ? std::size(*this) + x : x;
    }

  public:
    accumulation() {}

    template<class I, class Operator = std::plus<T>>
    accumulation(const I first, const I last, const T head = T{}, const Operator op = std::plus<T>{}) {
        this->assign(std::distance(first, last) + 1, {});
        std::exclusive_scan(first, last, std::begin(*this), head, op);
        if(this->size() > 1) *std::prev(this->end()) = op(*std::prev(std::end(*this), 2), *std::prev(last));
    }

    template<class Operaotr = std::minus<T>>
    inline T operator()(size_type left, size_type right, Operaotr op = std::minus<T>{}) const {
        left = _positivize_index(left), right = _positivize_index(right);
        assert(0 <= left and left <= right and right < (size_type)std::size(*this));
        return op((*this)[right], (*this)[left]);
    }
};


template<class T, class container = valarray<valarray<T>>, class Operator = std::plus<T>>
struct accumulation_2d : container {
    using size_type = internal::size_t;

  protected:
    inline size_type _positivize_index(const size_type x) const {
        return x < 0 ? std::size(*this) + x : x;
    }

    Operator _op;

  public:
    accumulation_2d() {}

    template<class I>
    accumulation_2d(const I first, const I last, const T head = T{}, const Operator op = std::plus<T>{}) : _op(op) {
        const size_type h = std::distance(first, last);
        const size_type w = std::distance(std::begin(*first), std::end(*first));
        {
            auto row = first;
            this->assign(h+1, head);
            (*this)[0].assign(w+1, head);
            REP(i, h) {
                assert(w == std::distance(std::begin(*row), std::end(*row)));
                (*this)[i+1].assign(w+1, head);
                REP(j, w) (*this)[i+1][j+1] = first[i][j];
                ++row;
            }
        }
        FOR(i, 1, h) FOR(j, w) (*this)[i][j] = op((*this)[i][j], (*this)[i-1][j]);
        FOR(i, h) FOR(j, 1, w) (*this)[i][j] = op((*this)[i][j], (*this)[i][j-1]);
    }

    template<class Rev = std::minus<T>>
    inline T operator()(size_type a, size_type b, size_type c, size_type d, const Rev rev = std::minus<T>{}) const {
        a = _positivize_index(a), b = _positivize_index(b);
        c = _positivize_index(c), d = _positivize_index(d);
        assert(0 <= a and a <= b and b < (size_type)std::size(*this));
        assert(0 <= c and c <= d and d < (size_type)std::size((*this)[0]));
        return this->_op(rev((*this)[b][d], this->_op((*this)[a][d], (*this)[b][c])), (*this)[a][c]);
    }

    template<class Rev = std::minus<T>>
    inline T operator()(const std::pair<size_type,size_type> p, const std::pair<size_type,size_type> q, const Rev rev = std::minus<T>{}) const {
        return this->operator()(p.first, p.second, q.first, q.second, rev);
    }
};



} // namespace lib
