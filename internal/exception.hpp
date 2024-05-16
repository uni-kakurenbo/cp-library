#pragma once


namespace uni {

namespace internal {


template<class... T> inline constexpr bool EXCEPTION_ON_TYPE = false;
template<auto T> inline constexpr bool EXCEPTION_ON_VALUE = false;


} // namespace internal

} // namespace uni
