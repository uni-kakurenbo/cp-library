#pragma once


namespace uni {

namespace internal {


template<int P> struct resolving_rank : resolving_rank<P-1> {};
template<> struct resolving_rank<0> {};


} // namespace internal

} // namespace uni
