#pragma once

#include <iterator>

#include "internal/types.hpp"

namespace Lib {

namespace Internal {

template<class T>
struct IIterator {
    using iterator_category = std::output_iterator_tag;

    using difference_type = Size;
    using value_type = T;

    using pointer = T*;
    using reference = T&;

    virtual T operator*() const = 0;
};

template<class T>
struct IBidirectionalIterator : IIterator<T> {
    using iterator_category = std::bidirectional_iterator_tag;

    virtual IBidirectionalIterator& operator++() = 0;
    virtual IBidirectionalIterator& operator--() = 0;
};

template<class T>
struct IRandomAccessIterator : IBidirectionalIterator<T> {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = typename IBidirectionalIterator<T>::difference_type;

  public:
    virtual IRandomAccessIterator& operator+=(const difference_type count) = 0;
    virtual IRandomAccessIterator& operator-=(const difference_type count) = 0;

    inline IRandomAccessIterator operator+(const difference_type count) const {
        auto res = *this;
        return res += count;
    }
    inline IRandomAccessIterator operator-(const difference_type count) const {
        auto res = *this;
        return res -= count;
    }
};

template<class T, class Container>
struct IContainerIterator : public IRandomAccessIterator<T> {
    using difference_type = typename IBidirectionalIterator<T>::difference_type;

  protected:
    const Container *const _ref;
    difference_type _pos;
    IContainerIterator(const Container *const ref, const difference_type& pos) : _ref(ref), _pos(pos) {}

  public:
    inline const Container * ref() const { return this->_ref; }

    inline difference_type pos() const { return this->_pos; }
    inline difference_type& pos() { return this->_pos; }

    inline IContainerIterator& operator++() override { return ++this->pos(), *this; }
    inline IContainerIterator& operator--() override { return --this->pos(), *this; }

    inline IContainerIterator& operator+=(const difference_type count) override { return this->pos() += count, *this; }
    inline IContainerIterator& operator-=(const difference_type count) override { return this->pos() -= count, *this; }

    inline difference_type operator-(const IContainerIterator& other) const { return this->pos() - other.pos(); }

    inline bool operator<(const IContainerIterator& other) const { return *this - other < 0; }
    inline bool operator>(const IContainerIterator& other) const { return *this - other > 0; }

    inline bool operator<=(const IContainerIterator& other) const { return not (*this > other); }
    inline bool operator>=(const IContainerIterator& other) const { return not (*this < other); }

    inline bool operator!=(const IContainerIterator& other) const { return this->ref() != other.ref() or *this < other or *this > other; }
    inline bool operator==(const IContainerIterator& other) const { return not (*this != other); }
};


} // namespace Internal

} // namespace Lib
