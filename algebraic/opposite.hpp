#pragma once


#include <concepts>
#include <utility>


#include "internal/dummy.hpp"

#include "algebraic/opposite.hpp"
#include "algebraic/internal/concepts.hpp"


namespace uni {

namespace algebraic {


template<internal::magma M>
struct opposite
  :
    std::conditional_t<internal::associative<M>, associative, uni::internal::dummy>,
    std::conditional_t<internal::commutative<M>, commutative, uni::internal::dummy>
{
    using value_type = M::value_type;

  private:
    M _value;

  public:
    template<std::convertible_to<M> T>
    opposite(const T& v) : _value(v) {};


    template<class... Args>
        requires std::constructible_from<value_type, Args...>
    opposite(Args&&... args) noexcept(NO_EXCEPT) : _value(std::forward<Args>(args)...) {}


    inline explicit operator value_type() const noexcept(NO_EXCEPT) { return this->_value.val(); }
    inline auto val() const noexcept(NO_EXCEPT) { return this->_value.val(); };

    inline const value_type* operator->() const noexcept(NO_EXCEPT) { return this->_value.operator->(); };
    inline value_type* operator->() noexcept(NO_EXCEPT) { return this->_value.operator->(); };


    friend inline auto operator<=>(const opposite& lhs, const opposite& rhs) noexcept(NO_EXCEPT) {
        return lhs._value <=> rhs._value;
    };

    friend inline bool operator==(const opposite& lhs, const opposite& rhs) noexcept(NO_EXCEPT) {
        return lhs._value == rhs._value;
    }


    friend inline opposite operator+(const opposite& lhs, const opposite& rhs) noexcept(NO_EXCEPT) {
        return rhs._value + lhs._value;
    }


    friend inline opposite operator-(const opposite& val) noexcept(NO_EXCEPT)
        requires internal::invertible<M>
    {
        return -val._value;
    }
};



template<internal::magma M>
struct make_opposite;


template<internal::magma M>
struct make_opposite<opposite<M>> {
    using type = M;
};

template<internal::magma M>
    requires internal::commutative<M>
struct make_opposite<M> {
    using type = M;
};


template<internal::magma M>
    requires (!internal::commutative<M>)
struct make_opposite<M> {
    using type = opposite<M>;
};


template<internal::magma M>
using make_opposite_t = make_opposite<M>::type;


} // namespace algebraic

} // namespace uni
