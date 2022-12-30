#pragma once

#include <iterator>

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

    virtual T operator*() const = 0;
};

template<class T>
struct bidirectiona_Iterator_interface : iterator_interface<T> {
    using iterator_category = std::bidirectional_iterator_tag;

    virtual bidirectiona_Iterator_interface& operator++() = 0;
    virtual bidirectiona_Iterator_interface& operator--() = 0;
};

template<class T>
struct random_access_iterator_base : bidirectiona_Iterator_interface<T> {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename bidirectiona_Iterator_interface<T>::difference_type;

  public:
    virtual random_access_iterator_base& operator+=(const difference_type count) = 0;
    virtual random_access_iterator_base& operator-=(const difference_type count) = 0;
};

template<class T, class container>
struct container_iterator_interface : public random_access_iterator_base<T> {
    using difference_type = typename bidirectiona_Iterator_interface<T>::difference_type;

  protected:
    const container *const _ref;
    difference_type _pos;
    container_iterator_interface(const container *const ref, const difference_type& pos) : _ref(ref), _pos(pos) {}

  public:
    inline const container * ref() const { return this->_ref; }

    inline difference_type pos() const { return this->_pos; }
    inline difference_type& pos() { return this->_pos; }

    inline container_iterator_interface& operator++() override { return ++this->pos(), *this; }
    inline container_iterator_interface& operator--() override { return --this->pos(), *this; }

    inline container_iterator_interface& operator+=(const difference_type count) override { return this->pos() += count, *this; }
    inline container_iterator_interface& operator-=(const difference_type count) override { return this->pos() -= count, *this; }

    inline difference_type operator-(const container_iterator_interface& other) const { return this->pos() - other.pos(); }

    inline bool operator<(const container_iterator_interface& other) const { return *this - other < 0; }
    inline bool operator>(const container_iterator_interface& other) const { return *this - other > 0; }

    inline bool operator<=(const container_iterator_interface& other) const { return not (*this > other); }
    inline bool operator>=(const container_iterator_interface& other) const { return not (*this < other); }

    inline bool operator!=(const container_iterator_interface& other) const { return this->ref() != other.ref() or *this < other or *this > other; }
    inline bool operator==(const container_iterator_interface& other) const { return not (*this != other); }
};


} // namespace internal

} // namespace lib
