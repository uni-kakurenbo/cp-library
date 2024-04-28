#pragma once


#include <iterator>
#include <concepts>


#include "internal/dev_env.hpp"

#include "global/constants.hpp"


namespace uni {

namespace internal {


template<class Super, std::integral SizeType = typename Super::size_type>
struct point_reference {
    using size_type = SizeType;
    using iterator = typename Super::iterator;

  protected:
    Super *const _super;
    const size_type _pos;

    point_reference(Super *const super, const size_type pos) noexcept(NO_EXCEPT) : _super(super), _pos(pos) {}

    inline auto index() noexcept(NO_EXCEPT) { return this->_pos; }
};


} // namespace internal

} // namespace uni
