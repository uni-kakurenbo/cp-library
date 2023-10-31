#pragma once


#include <cstdint>
#include "internal/bit_field.hpp"


namespace lib {

namespace actions {

namespace internal {


enum class tags : std::uint8_t {
    range_folding = 1 << 0,
    range_operation = 1 << 1,
};


} // namespace internal


struct flags : lib::internal::bit_field::base<internal::tags> {
    using lib::internal::bit_field::base<internal::tags>::base;

    static constexpr internal::tags range_folding = internal::tags::range_folding;
    static constexpr internal::tags range_operation = internal::tags::range_operation;
};


} // namespace actions

} // namespace lib
