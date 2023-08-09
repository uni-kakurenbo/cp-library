#pragma once


#ifndef $TYPE_TRAITS // for debuggers
#define $TYPE_TRAITS


#include <iostream>
#include <vector>
#include <type_traits>
#include <algorithm>
#include <utility>

#include "internal/dev_env.hpp"


namespace lib {

namespace internal {


template<class... Ts> struct tuple_or_pair { using type = std::tuple<Ts...>; };
template<class T, class U> struct tuple_or_pair<T,U> { using type = std::pair<T, U>; };
template <class... Ts> using tuple_or_pair_t = typename tuple_or_pair<Ts...>::type;

template<class T>
constexpr std::underlying_type_t<T> to_underlying(const T& v) noexcept(NO_EXCEPT) {
    return static_cast<std::underlying_type_t<T>>(v);
}


template<class T, class... Us>
using are_same = std::conjunction<std::is_same<T,Us>...>;

template<class T, class... Us>
inline constexpr bool are_same_v = std::conjunction_v<std::is_same<T,Us>...>;


template<class Base, class... Derived>
using are_base_of = std::conjunction<std::is_base_of<Base,Derived>...>;

template<class Base, class... Derived>
inline constexpr bool are_base_of_v = std::conjunction_v<std::is_same<Base,Derived>...>;


template<class T> struct remove_cvref {
  using type = typename std::remove_cv_t<std::remove_reference_t<T>>;
};

template<class T> using remove_cvref_t = typename remove_cvref<T>::type;


template<class T> struct literal_operator { static constexpr const char* value = ""; };
template<> struct literal_operator<unsigned> { static constexpr const char* value = "U"; };
template<> struct literal_operator<long> { static constexpr const char* value = "L"; };
template<> struct literal_operator<long double> { static constexpr const char* value = "L"; };
template<> struct literal_operator<unsigned long> { static constexpr const char* value = "UL"; };
template<> struct literal_operator<long long> { static constexpr const char* value = "LL"; };
template<> struct literal_operator<unsigned long long> { static constexpr const char* value = "ULL"; };

template<class T> inline constexpr const char* literal_operator_v = literal_operator<T>::value;


template<template <class...> class Template, class Arg> struct is_template : std::false_type {};
template<template <class...> class Template, class... Args> struct is_template<Template, Template<Args...>> : std::true_type {};

template<template <class...> class Template, class... Args>
inline constexpr bool  is_template_v = is_template<Template,Args...>::value;


template<class T> struct is_loggable {
    template<class U>
    static constexpr auto External(U &&v) -> decltype(_debug(v), std::true_type());
    static constexpr std::false_type External(...);

    template<class U>
    static constexpr auto Member(U &&v) -> decltype(v._debug(), std::true_type());
    static constexpr std::false_type Member(...);

    static constexpr bool value = (
      decltype(External(std::declval<T>()))::value ||
      decltype(Member(std::declval<T>()))::value
    );

};

template<class T> inline constexpr auto is_loggable_v = is_loggable<T>::value;


template<class T> struct _has_iterator {
    template<class U>
    static constexpr auto ADL(U &&v) -> decltype(begin(v), end(v), std::true_type());
    static constexpr std::false_type ADL(...);

    template<class U>
    static constexpr auto STL(U &&v) -> decltype(std::begin(v), std::end(v), std::true_type());
    static constexpr std::false_type STL(...);

    template<class U>
    static constexpr auto Member(U &&v) -> decltype(v.begin(), v.end(), std::true_type());
    static constexpr std::false_type Member(...);
};

template<class T> struct has_iterator {
  struct ADL : decltype(_has_iterator<T>::ADL(std::declval<T>())) {};
  struct STL : decltype(_has_iterator<T>::STL(std::declval<T>())) {};
  struct Member : decltype(_has_iterator<T>::Member(std::declval<T>())) {};

  static constexpr auto ADL_v = ADL::value;
  static constexpr auto STL_v = STL::value;
  static constexpr auto Member_v = Member::value;
};


template<class T> struct is_iterable {
    static constexpr bool value =  has_iterator<T>::ADL_v || has_iterator<T>::STL_v || has_iterator<T>::Member_v;
};

template<class T> inline constexpr auto is_iterable_v = is_iterable<T>::value;


template<class T> struct iterator_resolver {
  template<class U>
  static auto begin(U&& v) noexcept(NO_EXCEPT) {
    using U_t = remove_cvref_t<U>;

    static_assert(std::is_same_v<U_t, remove_cvref_t<T>>);
    static_assert(is_iterable_v<T>);

    if constexpr (has_iterator<T>::Member_v){
      return v.begin();
    } else {
      using std::begin;
      return begin(std::forward<U>(v));
    }
  }

  template<class U>
  static auto end(U&& v) noexcept(NO_EXCEPT) {
    using U_t = remove_cvref_t<U>;

    static_assert(std::is_same_v<U_t, remove_cvref_t<T>>);
    static_assert(is_iterable_v<T>);

    if constexpr (has_iterator<T>::Member_v){
      return v.end();
    } else {
      using std::end;
      return end(std::forward<U>(v));
    }
  }
};


template<class C> using iterator_t = decltype(iterator_resolver<C>::begin(std::declval<C&>()));

template<class C> using container_size_t = decltype(std::size(std::declval<C&>()));


template<bool Const, class T>
using maybe_const_t = std::conditional_t<Const, const T, T>;


#if CPP20

template<class T> using with_ref = T&;
template<class T> concept can_reference = requires { typename with_ref<T>; };

#endif

} // namespace internal

}  // namespace lib

#endif //ifndef $TYPE_TRAITS
