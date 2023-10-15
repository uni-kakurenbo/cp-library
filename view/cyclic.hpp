#pragma once


#include <cassert>
#include <functional>
#include <limits>
#include <ranges>
#include <type_traits>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "numeric/limits.hpp"

#include "view/internal/base.hpp"


namespace lib {


template<class Ref, class Size = std::make_signed_t<std::size_t>>
struct cyclic_view : internal::view_impl::base {
    using size_type = Size;
    using value_type = typename Ref::value_type;

    using transposer = std::function<size_type(const size_type)>;

  protected:
    Ref* _ref;
    size_type _internal_size;
    size_type _pos = 0, _taken = lib::numeric_limits<size_type>::arithmetic_infinity();

  private:
    template<class T> __attribute__((always_inline)) inline T _mod(const T v) const noexcept(NO_EXCEPT) {
        return (v + this->size()) % this->_internal_size;
    }

  public:
    cyclic_view(Ref& ref) noexcept(NO_EXCEPT) : _ref(&ref), _internal_size(std::distance(std::begin(ref), std::end(ref))) {}

    inline cyclic_view& reset(Ref& ref) noexcept(NO_EXCEPT) {
        this->_ref = &ref;
        this->_internal_size = std::distance(std::begin(ref), std::end(ref));
        return *this;
    }

    inline Ref* operator->() const noexcept { return this->_ref; }
    inline Ref& operator*() const noexcept { return *this->_ref; }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_taken; }

    inline cyclic_view& drop(const size_type pos = 0) noexcept(NO_EXCEPT) { this->_pos = pos; return *this; }
    inline cyclic_view& shift(const size_type count = 1) noexcept(NO_EXCEPT) { this->_pos += count; return *this; };

    inline cyclic_view& take(const size_type count) noexcept(NO_EXCEPT) { this->_taken = count; return *this; }

    inline value_type& operator[](const size_type count) noexcept {
        return this->_ref->operator[](this->_mod(this->_pos + count));
    }
    inline const value_type& operator[](const size_type count) const noexcept {
        return this->_ref->operator[](this->_mod(this->_pos + count));
    }

    struct iterator;

  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,cyclic_view,iterator>;

  public:
    struct iterator : iterator_interface {
        iterator(const cyclic_view *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->operator[](this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const noexcept(NO_EXCEPT) { return *(*this + count); }
    };

    inline iterator begin() noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }

    inline iterator end() noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace lib


namespace std::ranges {


template<class Ref>
inline constexpr bool enable_borrowed_range<lib::cyclic_view<Ref>> = true;


} // namespace std::ranges
