#pragma once

namespace lib {

namespace internal {


template<class... T> constexpr bool EXCEPTION = false;
template<const int T> constexpr bool EXCEPTION_INT = false;


} // namespace internal

} // namespace lib
