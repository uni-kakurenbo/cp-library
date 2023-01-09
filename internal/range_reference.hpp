#pragma once


namespace lib {

namespace internal {


template<class Super> struct range_reference {
    using size_type = typename Super::size_type;
    using iterator = typename Super::iterator;

  protected:
    const Super *const super;
    const size_type _begin, _end;

    range_reference(const Super *const super, const size_type begin, const size_type end) : super(super), _begin(begin), _end(end) {}

  public:
    inline iterator begin() const { return std::next(super->begin(), this->_begin); }
    inline iterator end() const { return std::next(super->begin(), this->_end); }

};


} // namespace internal

} // namespace lib
