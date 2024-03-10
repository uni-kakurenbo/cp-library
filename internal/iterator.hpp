#pragma once


#include <utility>
#include <type_traits>
#include <iterator>
#include <variant>
#include <ranges>


#include "internal/dev_env.hpp"

#include "internal/types.hpp"
#include "internal/type_traits.hpp"


namespace lib {

namespace internal {


template<class T>
struct iterator_interface {
    using iterator_category = std::output_iterator_tag;

    using difference_type = size_t;
    using value_type = T;

    using pointer = T*;
    using reference = T&;

    // virtual T operator*() const noexcept(NO_EXCEPT) { return 0; };
};

template<class T>
struct forward_iterator : iterator_interface<T> {
    using iterator_category = std::forward_iterator_tag;

    // virtual bidirectional_iterator_interface& operator++() = 0;
};

template<class T>
struct bidirectional_iterator_interface : forward_iterator<T> {
    using iterator_category = std::bidirectional_iterator_tag;

    // virtual bidirectional_iterator_interface& operator--() = 0;
};

template<class T>
struct random_access_iterator_base : bidirectional_iterator_interface<T> {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename bidirectional_iterator_interface<T>::difference_type;

  public:
    // virtual random_access_iterator_base& operator+=(const difference_type count) = 0;
    // virtual random_access_iterator_base& operator-=(const difference_type count) = 0;

    friend inline random_access_iterator_base operator+(random_access_iterator_base itr, const difference_type count) noexcept(NO_EXCEPT) { return itr += count, itr; }
    friend inline random_access_iterator_base operator-(random_access_iterator_base itr, const difference_type count) noexcept(NO_EXCEPT) { return itr -= count, itr; }

};

template<class T, class Container, class Derived>
struct container_iterator_interface : random_access_iterator_base<T> {
    using difference_type = typename bidirectional_iterator_interface<T>::difference_type;

  protected:
    Container* _ref;
    difference_type _pos;

    inline Derived* _derived() noexcept(NO_EXCEPT) {
        return static_cast<Derived*>(this);
    }
    inline const Derived* _derived() const noexcept(NO_EXCEPT) {
        return static_cast<const Derived*>(this);
    }

  public:
    container_iterator_interface() noexcept = default;
    container_iterator_interface(Container *const ref, const difference_type& pos) noexcept(NO_EXCEPT) : _ref(ref), _pos(pos) {}

    inline Container* ref() const noexcept(NO_EXCEPT) { return this->_ref; }

    inline difference_type pos() const noexcept(NO_EXCEPT) { return this->_pos; }
    inline difference_type& pos() { return this->_pos; }

    inline Derived& operator++() noexcept(NO_EXCEPT) { return ++this->_derived()->pos(), *this->_derived(); }
    inline Derived& operator--() noexcept(NO_EXCEPT) { return --this->_derived()->pos(), *this->_derived(); }

    inline Derived operator++(int) noexcept(NO_EXCEPT) { const auto res = *this->_derived(); return ++this->_derived()->pos(), res; }
    inline Derived operator--(int) noexcept(NO_EXCEPT) { const auto res = *this->_derived(); return --this->_derived()->pos(), res; }

    inline Derived& operator+=(const difference_type count) noexcept(NO_EXCEPT) { return this->pos() += count, *this->_derived(); }
    inline Derived& operator-=(const difference_type count) noexcept(NO_EXCEPT) { return this->pos() -= count, *this->_derived(); }

    inline auto operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->pos()); }
    inline T operator[](const difference_type count) const noexcept(NO_EXCEPT) { return *(*this->_derived() + count); }

    inline difference_type operator-(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return this->pos() - other.pos(); }

    inline bool operator<(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return *this->_derived() - other < 0; }
    inline bool operator>(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return *this->_derived() - other > 0; }

    inline bool operator<=(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return not (*this->_derived() > other); }
    inline bool operator>=(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return not (*this->_derived() < other); }

    inline bool operator==(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return this->_derived()->ref() == other.ref() and this->pos() == other.pos(); }
    inline bool operator!=(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return not (*this->_derived() == other); }
};

template<class V, std::input_or_output_iterator I>
inline auto to_non_const_iterator(V v, I itr) noexcept(NO_EXCEPT) {
    return std::ranges::next(std::ranges::begin(v), std::ranges::distance(std::ranges::cbegin(v), itr));
}


namespace iterator_impl {


template<class... Tags>
using is_all_random_access_iterator = is_base_of_all<std::random_access_iterator_tag,Tags...>;

template<class... Tags>
using is_all_bidirectional_iterator = is_base_of_all<std::bidirectional_iterator_tag,Tags...>;

template<class... Tags>
using is_all_forward_iterator = is_base_of_all<std::forward_iterator_tag,Tags...>;

template<class... Tags>
using is_all_input_iterator = is_base_of_all<std::input_iterator_tag,Tags...>;


template<class... Tags>
constexpr auto _most_primitive_iterator_tag() {
    if constexpr(is_all_random_access_iterator<Tags...>::value) {
        return std::random_access_iterator_tag{};
    }
    else if constexpr(is_all_bidirectional_iterator<Tags...>::value) {
        return std::bidirectional_iterator_tag{};
    }
    else if constexpr(is_all_forward_iterator<Tags...>::value) {
        return std::forward_iterator_tag{};
    }
    else {
        return std::input_iterator_tag{};
    }
}


} // namespace iterator_impl


template<class... Tags>
using most_primitive_iterator_tag = decltype(iterator_impl::_most_primitive_iterator_tag<Tags...>());


template<class T, class = void>
struct is_iterator {
   static constexpr bool value = false;
};

template<class T>
struct is_iterator<T, typename std::enable_if<!std::is_same<typename std::iterator_traits<T>::value_type, void>::value>::type> {
   static constexpr bool value = true;
};

template<class T>
constexpr bool is_iterator_v = is_iterator<T>::value;

template<class T>
using is_iterator_t = std::enable_if_t<is_iterator_v<T>>;

template<class T>
using iota_diff_t = std::make_signed_t<T>;


} // namespace internal

} // namespace lib
