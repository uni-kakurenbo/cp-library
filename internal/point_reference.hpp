#pragma once


#include <iterator>


#include "internal/dev_env.hpp"

#include "global/constants.hpp"


namespace lib {

namespace internal {


template<class Super> struct point_reference {
    using size_type = typename Super::size_type;
    using iterator = typename Super::iterator;

  protected:
    Super *const _super;
    const size_type _pos;

    point_reference(Super *const super, const size_type pos) noexcept(NO_EXCEPT) : _super(super), _pos(pos) {}
};


} // namespace internal

} // namespace lib
