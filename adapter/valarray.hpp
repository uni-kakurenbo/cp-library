#pragma once


#include <cassert>
#include <valarray>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <initializer_list>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"


namespace lib {


template<class T> struct valarray : std::valarray<T> {
    using size_type = internal::size_t;

    using iterator = T*;
    using const_iterator = const T*;

  protected:
    inline bool _validate_index_in_right_open([[maybe_unused]] const size_type p) const noexcept(NO_EXCEPT) {
        return 0 <= p and p < this->size();
    }
    inline bool _validate_index_in_closed([[maybe_unused]] const size_type p) const noexcept(NO_EXCEPT) {
        return 0 <= p and p <= this->size();
    }
    inline bool _validate_rigth_open_interval([[maybe_unused]] const size_type l, [[maybe_unused]] const size_type r) const noexcept(NO_EXCEPT) {
        return 0 <= l and l <= r and r <= this->size();
    }

    inline size_type _positivize_index(const size_type p) const noexcept(NO_EXCEPT) {
        return p < 0 ? this->size() + p : p;
    }

  public:
    valarray() noexcept(NO_EXCEPT) {}

    valarray(const std::size_t length, const T& val = T{}) noexcept(NO_EXCEPT) : std::valarray<T>(std::forward<const T>(val), length) {}

    template<class I, typename std::iterator_traits<I>::value_type* = nullptr>
    valarray(const I first, const I last) noexcept(NO_EXCEPT) : std::valarray<T>(first, std::distance(first, last)) {}

    template<class U> valarray(const std::initializer_list<U>& init) noexcept(NO_EXCEPT) : valarray(std::begin(init), std::end(init)) {}


    inline auto size() const noexcept(NO_EXCEPT) { return static_cast<size_type>(this->std::valarray<T>::size()); }

    inline void reserve(const size_type) noexcept(NO_EXCEPT) { /* do nothing */ }

    template<class I, typename std::iterator_traits<I>::value_type* = nullptr>
    inline void assign(const I first, const I last) noexcept(NO_EXCEPT) {
        this->resize(std::distance(first, last));
        std::copy(first, last, begin(*this));
    }

    inline void assign(const std::size_t length, const T& val = T{}) noexcept(NO_EXCEPT) {
        this->std::valarray<T>::resize(length, val);
    }

    inline void resize(const std::size_t length, const T& val = T{}) noexcept(NO_EXCEPT) {
        std::valarray<T> temp = *this;
        this->assign(length, val);
        std::move(std::begin(temp), std::min(std::end(temp), std::next(std::begin(temp), length)), std::begin(*this));
    }

    inline const T& operator[](size_type pos) const noexcept(NO_EXCEPT) {
        pos = this->_positivize_index(pos), assert(this->_validate_index_in_right_open(pos));
        return this->std::valarray<T>::operator[](pos);
    }
    inline T& operator[](size_type pos) noexcept(NO_EXCEPT) {
        pos = this->_positivize_index(pos), assert(this->_validate_index_in_right_open(pos));
        return this->std::valarray<T>::operator[](pos);
    }

    inline const T& back() const noexcept(NO_EXCEPT) { return *std::prev(this->end()); }
    inline T& back() { return *std::prev(this->end()); }

    inline const T& front() const noexcept(NO_EXCEPT) { return *this->begin(); }
    inline T& front() { return *this->begin(); }

    inline const T* begin() const noexcept(NO_EXCEPT) { return this->size() ? std::addressof((*this)[0]) : nullptr; }
    inline T* begin() { return this->size() ? std::addressof((*this)[0]) : nullptr; }

    inline const T* end() const noexcept(NO_EXCEPT) { if(auto n = this->size()) { return std::addressof((*this)[0]) + n; } else { return nullptr; } }
    inline T* end() { if(auto n = this->size()) { return std::addressof((*this)[0]) + n; } else { return nullptr; } }
};


} // namespace lib
