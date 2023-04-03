#pragma once


#include <iterator>

#include "constants.hpp"


namespace lib {

namespace internal {


template<class Super> struct range_reference {
    using size_type = typename Super::size_type;
    using iterator = typename Super::iterator;

  protected:
    Super *const _super;
    const size_type _begin, _end;

    range_reference(Super *const super, const size_type begin, const size_type end) : _super(super), _begin(begin), _end(end) {}

  public:
    inline iterator begin() const { return std::next(_super->begin(), this->_begin); }
    inline iterator end() const { return std::next(_super->begin(), this->_end); }

    inline size_type size() const { return this->_end - this->_begin; }

  protected:
    inline range_reference sub_range(size_type l, size_type r) const {
        l = _super->_positivize_index(l), r = _super->_positivize_index(r);
        dev_assert(0 <= l and l <= r and r <= this->size());

        return range_reference(_super, this->_begin + l, this->_begin + r);
    }

  public:
    template<lib::interval rng = lib::interval::right_open>
    inline range_reference range(const size_type l, const size_type r) const {
        if constexpr(rng == lib::interval::right_open) return this->sub_range(l, r);
        if constexpr(rng == lib::interval::left_open) return this->sub_range(l+1, r+1);
        if constexpr(rng == lib::interval::open) return this->sub_range(l+1, r);
        if constexpr(rng == lib::interval::closed) return this->sub_range(l, r+1);
    }
    inline range_reference range() const { return range_reference(this->_begin, this->_end); }

    inline range_reference operator()(const size_type l, const size_type r) const { return this->sub_range(l, r); }

    inline range_reference subseq(const size_type p, const size_type c) const { return this->sub_range(p, p+c); }
    inline range_reference subseq(const size_type p) const { return this->sub_range(p, this->size()); }
};


} // namespace internal

} // namespace lib
