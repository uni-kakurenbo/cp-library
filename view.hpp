#pragma once


#include <iterator>

#include "internal/types.hpp"

#include <vector>

namespace lib {


template<class I> struct view {
    using size_type = internal::size_t;
    using value_type = typename std::iterator_traits<I>::value_type;

  private:
    I _absolute_first, _begin, _end;
    size_type _size = 0;

  public:
    view(const I first) : _absolute_first(first), _begin(first), _end(first) {}
    view(const I first, const I last) : _absolute_first(first), _begin(first), _end(last) {}

    inline size_type size() const { return std::distance(this->_begin, this->_end); }

    inline I begin() const { return this->_begin; }
    inline I end() const { return this->_end; }

    inline view& focus(const I first) { this->_end = std::next(first, this->size()), this->_begin = first; return *this; }
    inline view& focus(const size_type first) { this->focus(std::next(this->_absolute_first, first)); return *this; }

    inline view& shift(const size_type count = 1) { std::advance(this->_begin, count), std::advance(this->_end, count); return *this; };

    inline view& resize(const size_type size) { this->_end = std::next(this->_begin, size); return *this; }

    inline value_type& operator[](const size_type count) { return *std::next(this->_begin, count); }
    inline const value_type& operator[](const size_type count) const { return *std::next(this->_begin, count); }
};


}
