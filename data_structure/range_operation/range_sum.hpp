#pragma once

#include "data_structure/core/fenwick_tree.hpp"

namespace Lib {


// Implemented by Fenwick Tree
template<class T> struct RangeSum : FenwickTree<T> {
   using Size = typename FenwickTree<T>::Size;
   using FenwickTree<T>::FenwickTree;

   inline auto sum(const Size first, const Size last) const { return this->prod(first, last); }
};


} // namespace Lib
