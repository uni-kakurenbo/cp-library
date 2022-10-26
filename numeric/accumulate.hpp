#pragma once

#include <iterator>
#include <vector>
#include <functional>
#include <numeric>

template<class I, class T = typename std::iterator_traits<I>::value_type>
struct Accumulate : std::vector<T> {
    Accumulate() {}
    template<class Operator = std::plus<T>> Accumulate(const I first, const I last, const T head = T(), const Operator op = std::plus<T>{}) {
        this->resize(distance(first,last));
        std::exclusive_scan(first,last,this->begin(), head, op);
        this->push_back(op(*this->rbegin(), *prev(last)));
    }
    inline T operator()(const size_t left, const size_t right) const {
        return (*this)[right] - (*this)[left];
    }
};
