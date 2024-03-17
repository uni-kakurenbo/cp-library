#pragma once


namespace lib {

namespace internal {


template<class... T> inline constexpr bool EXCEPTION = false;
template<const int T> inline constexpr bool EXCEPTION_INT = false;


} // namespace internal

} // namespace lib
