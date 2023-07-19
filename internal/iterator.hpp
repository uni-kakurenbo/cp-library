#pragma once

#include <utility>
#include <type_traits>
#include <iterator>
#include <iterator>
#include <variant>

#include "internal/dev_env.hpp"

#include "internal/types.hpp"


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
};

template<class T, class container>
struct container_iterator_interface : public random_access_iterator_base<T> {
    using difference_type = typename bidirectional_iterator_interface<T>::difference_type;

  protected:
    const container *const _ref;
    difference_type _pos;
    container_iterator_interface(const container *const ref, const difference_type& pos) noexcept(NO_EXCEPT) : _ref(ref), _pos(pos) {}

  public:
    inline const container* ref() const noexcept(NO_EXCEPT) { return this->_ref; }

    inline difference_type pos() const noexcept(NO_EXCEPT) { return this->_pos; }
    inline difference_type& pos() { return this->_pos; }

    inline container_iterator_interface& operator++() noexcept(NO_EXCEPT) { return ++this->pos(), *this; }
    inline container_iterator_interface& operator--() noexcept(NO_EXCEPT) { return --this->pos(), *this; }

    inline container_iterator_interface operator++(int) noexcept(NO_EXCEPT) { const auto res = *this; return ++this->pos(), res; }
    inline container_iterator_interface operator--(int) noexcept(NO_EXCEPT) { const auto res = *this; return --this->pos(), res; }

    inline container_iterator_interface& operator+=(const difference_type count) noexcept(NO_EXCEPT) { return this->pos() += count, *this; }
    inline container_iterator_interface& operator-=(const difference_type count) noexcept(NO_EXCEPT) { return this->pos() -= count, *this; }

    inline difference_type operator-(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return this->pos() - other.pos(); }

    inline bool operator<(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return *this - other < 0; }
    inline bool operator>(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return *this - other > 0; }

    inline bool operator<=(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return not (*this > other); }
    inline bool operator>=(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return not (*this < other); }

    inline bool operator!=(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return this->ref() != other.ref() or *this < other or *this > other; }
    inline bool operator==(const container_iterator_interface& other) const noexcept(NO_EXCEPT) { return not (*this != other); }
};


template<class I, std::enable_if_t<std::is_base_of_v<random_access_iterator_base<typename I::value_type>,I>>* = nullptr>
inline I operator+(I itr, const typename I::difference_type count) noexcept(NO_EXCEPT) { return itr += count, itr; }

template<class I, std::enable_if_t<std::is_base_of_v<random_access_iterator_base<typename I::value_type>,I>>* = nullptr>
inline I operator-(I itr, const typename I::difference_type count) noexcept(NO_EXCEPT) { return itr -= count, itr; }

template<class V, class I>
inline auto to_non_const_iterator(V v, const I itr) noexcept(NO_EXCEPT) { return std::next(std::begin(v), std::distance(std::cbegin(v), itr)); }


} // namespace internal

} // namespace lib
