#pragma once


#include <cstdint>
#include "internal/bit_field.hpp"


namespace lib {

namespace actions {

namespace internal {


enum class tags : std::int8_t {
    fenwick_tree = 1 << 0,
    segment_tree = 1 << 1,
    lazy_segment_tree = 1 << 2,
    implicit_treap = 1 << 3,
};


} // namespace internal


struct flags : lib::internal::bit_field::base<internal::tags> {
    using lib::internal::bit_field::base<internal::tags>::base;

    static constexpr internal::tags fenwick_tree = internal::tags::fenwick_tree;
    static constexpr internal::tags segment_tree = internal::tags::segment_tree;
    static constexpr internal::tags lazy_segment_tree = internal::tags::lazy_segment_tree;
    static constexpr internal::tags implicit_treap = internal::tags::implicit_treap;
};


} // namespace actions

} // namespace lib
