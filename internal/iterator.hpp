#pragma once


#include <utility>
#include <type_traits>
#include <iterator>
#include <variant>
#include <compare>
#include <ranges>


#include "internal/dev_env.hpp"

#include "internal/types.hpp"
#include "internal/type_traits.hpp"


namespace uni {

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
    using difference_type = std::make_signed_t<typename Container::size_type>;

  private:
    using derived = std::remove_cvref_t<Derived>;

    Container* _ref;
    difference_type _pos;

    static_assert(std::three_way_comparable<difference_type>);

    inline auto* _derived() noexcept(NO_EXCEPT) {
        return static_cast<derived*>(this);
    }
    inline const auto* _derived() const noexcept(NO_EXCEPT) {
        return static_cast<const derived*>(this);
    }

  public:
    container_iterator_interface() noexcept = default;
    container_iterator_interface(Container *const ref, const difference_type pos) noexcept(NO_EXCEPT) : _ref(ref), _pos(pos) {}

    inline auto ref() const noexcept(NO_EXCEPT) { return this->_ref; }

    inline auto pos() const noexcept(NO_EXCEPT) { return this->_pos; }
    inline auto& pos() { return this->_pos; }

    inline auto& operator++() noexcept(NO_EXCEPT) { return ++this->_pos, *this->_derived(); }
    inline auto& operator--() noexcept(NO_EXCEPT) { return --this->_pos, *this->_derived(); }

    inline auto operator++(int) noexcept(NO_EXCEPT) { auto res = *this->_derived(); return ++this->_pos, res; }
    inline auto operator--(int) noexcept(NO_EXCEPT) { auto res = *this->_derived(); return --this->_pos, res; }

    inline auto& operator+=(const difference_type count) noexcept(NO_EXCEPT) { return this->_pos += count, *this->_derived(); }
    inline auto& operator-=(const difference_type count) noexcept(NO_EXCEPT) { return this->_pos -= count, *this->_derived(); }

    inline auto operator*() const noexcept(NO_EXCEPT) { return this->ref()->get(this->_pos); }
    inline auto operator[](const difference_type count) const noexcept(NO_EXCEPT) { return *(*this->_derived() + count); }

    inline auto operator-(const derived& other) const noexcept(NO_EXCEPT) { return this->_pos - other._pos; }


    friend inline bool operator==(const derived& lhs, const derived& rhs) noexcept(NO_EXCEPT) {
        if(lhs.ref() == rhs.ref()) return lhs._pos == rhs._pos;
        return false;
    }

    friend inline std::partial_ordering operator<=>(const derived& lhs, const derived& rhs) noexcept(NO_EXCEPT) {
        if(lhs.ref() != rhs.ref()) return std::partial_ordering::unordered;
        return lhs._pos <=> rhs._pos;
    }
};


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
inline constexpr bool is_iterator_v = is_iterator<T>::value;

template<class T>
using is_iterator_t = std::enable_if_t<is_iterator_v<T>>;

template<class T>
using iota_diff_t = std::make_signed_t<T>;


} // namespace internal

} // namespace uni
