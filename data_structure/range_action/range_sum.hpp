#pragma once

#include "data_structure/core/fenwick_tree.hpp"

namespace lib {


// Implemented by Fenwick Tree
template<class T> struct range_sum : fenwick_tree<T> {
   using size_t = typename fenwick_tree<T>::size_t;
   using fenwick_tree<T>::fenwick_tree;

   inline auto sum(const size_t first, const size_t last) const { return this->prod(first, last); }
};


} // namespace lib
