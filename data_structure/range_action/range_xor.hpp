#pragma once

#include "data_structure/core/fenwick_tree.hpp"

namespace lib {


template<class T> struct range_xor : fenwick_tree<T,internal::bxor<T>,internal::bxor<T>> {
   using size_t = typename fenwick_tree<T>::size_t;
   using fenwick_tree<T>::fenwick_tree;
};


} // namespace lib
