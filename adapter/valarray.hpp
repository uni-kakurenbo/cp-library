#pragma once


#include <cassert>
#include <iostream>
#include <valarray>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <initializer_list>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adapter/internal/container_extender.hpp"


namespace lib {


template<class T> struct valarray : internal::extended_container<std::valarray<T>> {
  private:
    using base = internal::extended_container<std::valarray<T>>;

  public:
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

    explicit valarray(const std::size_t length, const T& val = T{}) noexcept(NO_EXCEPT) : base(std::forward<const T>(val), length) {}

    template<std::input_iterator I, std::sentinel_for<I> S>
    valarray(I first, S last) noexcept(NO_EXCEPT) : base(std::ranges::distance(first, last)) { std::ranges::copy(first, last, std::ranges::begin(*this)); }

    template<class U> valarray(const U* pointer, const size_t n) noexcept(NO_EXCEPT) : base(pointer, n) {};

    valarray(const std::slice_array<T>& arr) noexcept(NO_EXCEPT) : base(arr) {};
    valarray(const std::gslice_array<T>& arr) noexcept(NO_EXCEPT) : base(arr) {};
    valarray(const std::mask_array<T>& arr) noexcept(NO_EXCEPT) : base(arr) {};
    valarray(const std::indirect_array<T>& arr) noexcept(NO_EXCEPT) : base(arr) {};
    valarray(const std::initializer_list<T>& init) noexcept(NO_EXCEPT) : base(init) {}

  #ifdef __GNUC__
    template<class Dom> valarray(const std::_Expr<Dom,T>& expr) noexcept(NO_EXCEPT) : base(expr) {}
  #endif

    inline auto size() const noexcept(NO_EXCEPT) { return static_cast<size_type>(this->base::size()); }

    inline void reserve(const size_type) noexcept(NO_EXCEPT) { /* do nothing */ }

    template<std::input_iterator I, std::sentinel_for<I> S>
    inline void assign(I first, S last) noexcept(NO_EXCEPT) {
        this->resize(std::ranges::distance(first, last));
        std::ranges::copy(first, last, std::ranges::begin(*this));
    }

    inline void assign(const std::size_t length, const T& val = T{}) noexcept(NO_EXCEPT) {
        this->base::resize(length, val);
    }

    inline void resize(const std::size_t length, const T& val = T{}) noexcept(NO_EXCEPT) {
        base temp = *this;
        this->assign(length, val);
        std::move(std::begin(temp), std::min(std::end(temp), std::next(std::begin(temp), length)), std::begin(*this));
    }

    inline const T& operator[](size_type pos) const noexcept(NO_EXCEPT) {
        pos = this->_positivize_index(pos), assert(this->_validate_index_in_right_open(pos));
        return this->base::operator[](pos);
    }
    inline T& operator[](size_type pos) noexcept(NO_EXCEPT) {
        pos = this->_positivize_index(pos), assert(this->_validate_index_in_right_open(pos));
        return this->base::operator[](pos);
    }

    inline const T& back() const noexcept(NO_EXCEPT) { return *std::prev(this->end()); }
    inline T& back() noexcept(NO_EXCEPT) { return *std::prev(this->end()); }

    inline const T& front() const noexcept(NO_EXCEPT) { return *this->begin(); }
    inline T& front() noexcept(NO_EXCEPT) { return *this->begin(); }

    inline const T* begin() const noexcept(NO_EXCEPT) { return this->size() ? std::addressof((*this)[0]) : nullptr; }
    inline T* begin() noexcept(NO_EXCEPT) { return this->size() ? std::addressof((*this)[0]) : nullptr; }

    inline const T* end() const noexcept(NO_EXCEPT) { if(auto n = this->size()) { return std::addressof((*this)[0]) + n; } else { return nullptr; } }
    inline T* end() noexcept(NO_EXCEPT) { if(auto n = this->size()) { return std::addressof((*this)[0]) + n; } else { return nullptr; } }

    inline auto rbegin() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(std::end(*this)); }
    inline auto rend() noexcept(NO_EXCEPT) { return std::make_reverse_iterator(std::begin(*this)); }

    inline auto rbegin() const noexcept(NO_EXCEPT) { return std::make_reverse_iterator(std::end(*this)); }
    inline auto rend() const noexcept(NO_EXCEPT) { return std::make_reverse_iterator(std::begin(*this)); }
};


} // namespace lib
