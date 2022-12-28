#pragma once

#include "data_structure/core/fenwick_tree.hpp"

namespace Lib {


template<class T> struct RangeXor : FenwickTree<T,Internal::bxor<T>,Internal::bxor<T>> {
   using Size = typename FenwickTree<T>::Size;
   using FenwickTree<T>::FenwickTree;
};


} // namespace Lib
