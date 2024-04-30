#pragma once


#include <cassert>
#include <iterator>
#include <concepts>
#include <ranges>


#include "internal/dev_env.hpp"

#include "global/constants.hpp"


namespace uni {

namespace internal {


template<class Super, std::integral SizeType = typename Super::size_type>
struct range_reference {
    using size_type = SizeType;
    using iterator = Super::iterator;

  protected:
    Super *const _super;
    const size_type _begin, _end;

    range_reference(Super *const super, const size_type begin, const size_type end) noexcept(NO_EXCEPT) : _super(super), _begin(begin), _end(end) {}

  public:
    inline auto begin() const noexcept(NO_EXCEPT) { return std::ranges::next(std::ranges::begin(*this->_super), this->_begin); }
    inline auto end() const noexcept(NO_EXCEPT) { return std::ranges::next(std::ranges::begin(*this->_super), this->_end); }

    inline auto size() const noexcept(NO_EXCEPT) { return this->_end - this->_begin; }

    inline auto interval() const noexcept(NO_EXCEPT) { return std::make_pair(this->_begin, this->_end); }

  protected:
    inline auto sub_range(size_type l, size_type r) const noexcept(NO_EXCEPT) {
        l = _super->_positivize_index(l), r = _super->_positivize_index(r);
        assert(0 <= l and l <= r and r <= this->size());

        return range_reference(_super, this->_begin + l, this->_begin + r);
    }

  public:
    template<uni::interval_notation rng = uni::interval_notation::right_open>
    inline auto range(const size_type l, const size_type r) const noexcept(NO_EXCEPT) {
        if constexpr(rng == uni::interval_notation::right_open) return this->sub_range(l, r);
        if constexpr(rng == uni::interval_notation::left_open) return this->sub_range(l+1, r+1);
        if constexpr(rng == uni::interval_notation::open) return this->sub_range(l+1, r);
        if constexpr(rng == uni::interval_notation::closed) return this->sub_range(l, r+1);
    }
    inline auto range() const noexcept(NO_EXCEPT) { return range_reference(this->_begin, this->_end); }

    inline auto operator()(const size_type l, const size_type r) const noexcept(NO_EXCEPT) { return this->sub_range(l, r); }

    inline auto subseq(const size_type p, const size_type c) const noexcept(NO_EXCEPT) { return this->sub_range(p, p+c); }
    inline auto subseq(const size_type p) const noexcept(NO_EXCEPT) { return this->sub_range(p, this->size()); }
};


} // namespace internal

} // namespace uni
