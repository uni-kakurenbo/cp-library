#pragma once

namespace Lib {

namespace Internal {

template<int P> struct Rank : Rank<P-1> {};
template<> struct Rank<0> {};

} // namespace Internal

} // namespace Lib
