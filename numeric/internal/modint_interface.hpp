#pragma once


#include <concepts>
#include <type_traits>
#include <atcoder/modint>


#include "snippet/aliases.hpp"

#include "internal/concepts.hpp"


namespace lib {

namespace internal {


template<structural Derived, std::unsigned_integral Value>
    requires std::same_as<std::remove_cv_t<Derived>, Derived>
struct modint_interface {
  private:
    inline constexpr Derived* _derived() noexcept(NO_EXCEPT) {
        return static_cast<Derived*>(this);
    }
    inline constexpr const Derived* _derived() const noexcept(NO_EXCEPT) {
        return static_cast<const Derived*>(this);
    }


  public:
    modint_interface() noexcept = default;

    explicit inline operator Value() const noexcept(NO_EXCEPT)
        requires requires (Derived v) { v.val(); }
    {
        return this->_derived()->val();
    }

    inline constexpr auto& operator++() noexcept(NO_EXCEPT)
        requires requires (Derived v) { v += Derived::one; }
    {
        return *this->_derived() += Derived::one;
    }

    inline constexpr auto& operator--() noexcept(NO_EXCEPT)
        requires requires (Derived v) { v -= Derived::one; }
    {
        return *this->_derived() -= Derived::one;
    }


    inline constexpr auto operator++(int) noexcept(NO_EXCEPT)
        requires requires (Derived v) { ++v; }
    {
        Derived res = *this->_derived();
        return ++*this->_derived(), res;
    }

    inline constexpr auto operator--(int) noexcept(NO_EXCEPT)
        requires requires (Derived v) { --v; }
    {
        Derived res = *this->_derived();
        return --*this->_derived(), res;
    }


    inline constexpr auto operator+() const noexcept(NO_EXCEPT) { return *this->_derived(); }

    inline constexpr auto operator-() const noexcept(NO_EXCEPT)
        requires requires (const Derived v) { Derived::zero - v; }
    {
        return Derived::zero - *this->_derived();
    }

    inline constexpr auto& operator/=(const Derived& rhs) noexcept(NO_EXCEPT)
        requires requires (Derived v) { v *= v.inv(); }
    {
        return *this->_derived() *= rhs.inv();
    }


    constexpr auto pow(i64 n) const noexcept(NO_EXCEPT)
        requires multipliation_assignalbe<Derived> && requires { Derived::one; }
    {
        if(Derived::mod() == 1) return Derived::zero;
        return lib::pow(*this->_derived(), n);
    }


    constexpr Derived inv() const noexcept(NO_EXCEPT)
        requires
            requires(Derived v, int n) {
                v.val();
                Derived::mod();
                Derived::raw(n);
            }
    {
        using signed_value_type = std::make_signed_t<Value>;

        signed_value_type x = this->_derived()->val(), y = Derived::mod(), u = 1, v = 0;
        while(y > 0) {
            signed_value_type t = x / y;
            std::swap(x -= t * y, y);
            std::swap(u -= t * v, v);
        }
        assert(x == 1);

        if(u < 0) u += v / x;
        return Derived::raw(u);
    }


    friend inline constexpr bool operator!=(const Derived& lhs, const Derived& rhs) noexcept(NO_EXCEPT)
        requires requires(Derived v) { v == v; }
    {
        return !(lhs == rhs);
    }

    friend inline constexpr auto operator+(Derived lhs, const Derived& rhs) noexcept(NO_EXCEPT)
#if __GNUC__ < 13
        requires addition_assignable<Derived>
#endif
    {
        return lhs += rhs;
    }

    friend inline constexpr auto operator-(Derived lhs, const Derived& rhs) noexcept(NO_EXCEPT)
#if __GNUC__ < 13
        requires subtraction_assignable<Derived>
#endif
    {
        return lhs -= rhs;
    }

    friend inline constexpr auto operator*(Derived lhs, const Derived& rhs) noexcept(NO_EXCEPT)
#if __GNUC__ < 13
        requires multipliation_assignalbe<Derived>
#endif
    {
        return lhs *= rhs;
    }

    friend inline constexpr auto operator/(Derived lhs, const Derived& rhs) noexcept(NO_EXCEPT)
#if __GNUC__ < 13
        requires division_assignable<Derived>
#endif
    {
        return lhs /= rhs;
    }
};



template<std::unsigned_integral Value, std::unsigned_integral Large, Value Mod>
    requires has_double_digits_of<Large, Value> && (Mod > 0)
struct static_modint_impl;


template<std::unsigned_integral Value, std::unsigned_integral Large, i64 Id>
    requires has_double_digits_of<Large, Value>
struct montgomery_modint_impl;


template<std::unsigned_integral Value, std::unsigned_integral Large, i64 Id>
    requires has_double_digits_of<Large, Value>
struct barrett_modint_impl;


template<std::unsigned_integral Value, i64 Id>
struct binary_modint_impl;


using atcoder::internal::is_modint;
template<class T> constexpr bool is_modint_v = is_modint<T>::value;
using atcoder::internal::is_modint_t;


template<class T> concept modint_family =
    numeric<T> &&
    has_static_one<T> && has_static_zero<T> &&
    requires (T v, i64 p, typename T::unsigned_value_type x) {
        { v.pow(p) } -> std::same_as<T>;
        { v.inv() } -> std::same_as<T>;
        { T::raw(x) } -> std::same_as<T>;

        { v.val() } -> std::same_as<typename T::unsigned_value_type>;
        { T::mod() } -> std::same_as<typename T::unsigned_value_type>;
        { T::max() } -> std::same_as<typename T::unsigned_value_type>;
        T::digits;
    };


template<class T>
concept dynamic_modint_family =
    modint_family<T> &&
    requires (typename T::unsigned_value_type v) {
        T::set_mod(v);
    };


template<class T>
concept static_modint_family =
    modint_family<T> &&
    requires {
        T::is_prime;
    };


} // namespace internal


template<u32 Mod> using static_modint_32bit = internal::static_modint_impl<u32, u64, Mod>;
template<u64 Mod> using static_modint_64bit = internal::static_modint_impl<u64, u128, Mod>;

template<i64 Id> using barrett_modint_32bit = internal::barrett_modint_impl<u32, u64, Id>;
template<i64 Id> using barrett_modint_64bit = internal::barrett_modint_impl<u64, u128, Id>;

template<i64 Id> using montgomery_modint_32bit = internal::montgomery_modint_impl<u32, u64, Id>;
template<i64 Id> using montgomery_modint_64bit = internal::montgomery_modint_impl<u64, u128, Id>;

template<i64 Id> using binary_modint_32bit = internal::binary_modint_impl<u32, Id>;
template<i64 Id> using binary_modint_64bit = internal::binary_modint_impl<u64, Id>;
template<i64 Id> using binary_modint_128bit = internal::binary_modint_impl<u128, Id>;


using modint998244353 = static_modint_32bit<998244353>;
using modint1000000007 = static_modint_32bit<1000000007>;

using modint_32 = barrett_modint_32bit<-1>;
using modint_64 = barrett_modint_64bit<-1>;


template<const unsigned Val, const unsigned Mod = 998244353>
const lib::static_modint_32bit<Mod> MINT = Val;

template<const unsigned Val, const unsigned Mod = 998244353>
const unsigned INV = lib::static_modint_32bit<Mod>{ Val }.inv().val();

template<const unsigned Val, const unsigned Mod = 998244353>
const int SINV = lib::static_modint_32bit<Mod>{ Val }.inv().val();


} // namespace lib
