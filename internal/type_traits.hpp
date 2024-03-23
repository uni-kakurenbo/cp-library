#pragma once


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


template<class T, class... Ts>
using are_same = std::conjunction<std::is_same<T, Ts>...>;

template<class T, class... Ts>
inline constexpr bool are_same_v = are_same<T, Ts...>::value;


template<class T, class... Ts>
using is_same_as_any_of = std::disjunction<std::is_same<T, Ts>...>;

template<class T, class... Ts>
inline constexpr bool is_same_as_any_of_v = is_same_as_any_of<T, Ts...>::value;

template<class T, class... Ts>
concept same_as_any_of = is_same_as_any_of_v<T, Ts...>;


template<class Base, class... Derived>
using is_base_of_all = std::conjunction<std::is_base_of<Base, Derived>...>;

template<class Base, class... Derived>
inline constexpr bool is_base_of_all_v = is_base_of_all<Base, Derived...>::value;


template<class Base, class... Derived>
using is_base_of_any = std::disjunction<std::is_base_of<Base, Derived>...>;

template<class Base, class... Derived>
inline constexpr bool is_base_of_any_v = is_base_of_any<Base, Derived...>::value;


template<class T> struct remove_cvref {
  using type = typename std::remove_cv_t<std::remove_reference_t<T>>;
};

template<class T> using remove_cvref_t = typename remove_cvref<T>::type;


template<class T> struct literal_operator { static constexpr const char* value = ""; };

template<> struct literal_operator<unsigned> { static constexpr const char* value = "U"; };
template<> struct literal_operator<long> { static constexpr const char* value = "L"; };
template<> struct literal_operator<unsigned long> { static constexpr const char* value = "UL"; };
template<> struct literal_operator<long long> { static constexpr const char* value = "LL"; };
template<> struct literal_operator<unsigned long long> { static constexpr const char* value = "ULL"; };


template<> struct literal_operator<float> { static constexpr const char* value = "F"; };
template<> struct literal_operator<double> { static constexpr const char* value = "D"; };
template<> struct literal_operator<long double> { static constexpr const char* value = "LD"; };

#ifdef __SIZEOF_INT128__

template<> struct literal_operator<__int128_t> { static constexpr const char* value = "LLL"; };
template<> struct literal_operator<__uint128_t> { static constexpr const char* value = "ULLL"; };

#endif

template<class T> inline constexpr auto literal_operator_v = literal_operator<T>::value;


template <std::size_t N, typename... Types>
struct nth_type {};

template <class Head, class... Tail>
struct nth_type<0, Head, Tail...> {
	using type = Head;
};

template <std::size_t N, class Head, class... Tail>
struct nth_type<N, Head, Tail...> : public nth_type<N - 1, Tail...> {};

template <std::size_t N, typename... Types>
using nth_type_t = typename nth_type<N, Types...>::type;


template<template <class...> class, class> struct is_template_of : std::false_type {};
template<template <class...> class Template, class... Args> struct is_template_of<Template, Template<Args...>> : std::true_type {};

template<template <class...> class Template, class Type>
inline constexpr bool is_template_of_v = is_template_of<Template, Type>::value;

template<class Type, template <class...> class Template>
concept substituted_from = is_template_of_v<Template, Type>;

template<template <class...> class Base, class Derived>
struct _is_basic_tempalte_of
{
    template<class... Ts>
    static constexpr std::true_type  test(const Base<Ts...> *);

    static constexpr std::false_type test(...);

    using type = decltype(test(std::declval<Derived*>()));
};


template<template <class...> class Base, class Derived>
using is_basic_tempalte_of = _is_basic_tempalte_of<Base, Derived>::type;

template<template <class...> class Base, class Derived>
inline constexpr bool is_basic_tempalte_of_v = is_basic_tempalte_of<Base, Derived>::value;

template<class Derived, template <class...> class Base>
concept derived_from_template = is_basic_tempalte_of_v<Base, Derived>;


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

template<class T>
inline constexpr auto is_loggable_v = is_loggable<T>::value;

template<class T>
concept loggable = is_loggable_v<T>;


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

    static constexpr auto adl_v = ADL::value;
    static constexpr auto stl_v = STL::value;
    static constexpr auto member_v = Member::value;
};


template<class T>
struct is_iterable {
    static constexpr bool value =  has_iterator<T>::adl_v || has_iterator<T>::stl_v || has_iterator<T>::member_v;
};

template<class T>
inline constexpr auto is_iterable_v = is_iterable<T>::value;

template<class T>
concept iterable = is_iterable_v<T>;

namespace iterator_resolver {


template<class T>
inline constexpr auto begin(T&& v) noexcept(NO_EXCEPT) {
    static_assert(is_iterable_v<T>);
    if constexpr(has_iterator<T>::member_v) {
        return v.begin();
    }
    else {  // ADL
        using std::begin;
        return begin(std::forward<T>(v));
    }
}

template<class T>
inline constexpr auto end(T&& v) noexcept(NO_EXCEPT) {
    static_assert(is_iterable_v<T>);
    if constexpr(has_iterator<T>::member_v) {
        return v.end();
    }
    else {  // ADL
        using std::end;
        return end(std::forward<T>(v));
    }
}


};


template<class C> using iterator_t = decltype(iterator_resolver::begin(std::declval<C&>()));

template<class C> using container_size_t = decltype(std::size(std::declval<C&>()));


template<bool Const, class T>
using maybe_const_t = std::conditional_t<Const, const T, T>;


template<class T> using with_ref = T&;
template<class T> concept can_reference = requires { typename with_ref<T>; };


} // namespace internal

}  // namespace lib
