#pragma once


#include <cassert>
#include <functional>

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/iterator.hpp"

#include "view/internal/base.hpp"


namespace lib {


template<class Ref>
struct view : internal::view_impl::base {
    using size_type = internal::size_t;
    using value_type = typename Ref::value_type;

    using transposer = std::function<size_type(const size_type)>;

  protected:
    transposer _transposer = [&](const size_type pos) { return pos; };

    Ref* _ref;
    size_type _pos = 0, _taken = 0;

  private:
    __attribute__((always_inline)) inline void _validate() const noexcept(NO_EXCEPT) {
        assert(0 <= this->_pos && this->_pos < static_cast<size_type>(this->_ref->size()));
        assert(0 <= this->_taken);
        assert(this->_pos + this->_taken <= static_cast<size_type>(this->_ref->size()));
    }

  public:
    view(Ref *const ref) noexcept(NO_EXCEPT) : _ref(ref) {}

    inline view& reset(Ref *const ref) noexcept(NO_EXCEPT) { this->_ref = ref; return *this; }

    inline Ref* operator->() const noexcept { return this->_ref; }
    inline Ref& operator*() const noexcept { return *this->_ref; }

    inline size_type size() const noexcept(NO_EXCEPT) { return this->_taken; }

    // inline auto begin() const noexcept(NO_EXCEPT) { return std::next(std::begin(*this->_ref), this->_pos); }
    // inline auto end() const noexcept(NO_EXCEPT) { return std::next(this->begin(), this->_taken); }

    inline view& drop(const size_type pos = 0) noexcept(NO_EXCEPT) { this->_pos = pos; this->_validate(); return *this; }
    inline view& shift(const size_type count = 1) noexcept(NO_EXCEPT) { this->_pos += count; this->_validate(); return *this; };

    inline view& take(const size_type count) noexcept(NO_EXCEPT) { this->_taken = count; this->_validate(); return *this; }
    inline view& take() noexcept(NO_EXCEPT) { this->_taken = std::size(*this->_ref); return *this; }

    inline view& transpose(const transposer& f) noexcept { this->_transposer = f; return *this; }


    inline value_type& operator[](const size_type count) noexcept {
        this->_validate();
        return std::next(std::begin(*this->_ref), this->_pos)[this->_transposer(count)];
    }
    inline const value_type& operator[](const size_type count) const noexcept {
        this->_validate();
        return std::next(std::begin(*this->_ref), this->_pos)[this->_transposer(count)];
    }


  protected:
    using iterator_interface = internal::container_iterator_interface<value_type,view>;

  public:
    struct iterator : virtual iterator_interface {
        iterator(const view *const ref, const size_type pos) noexcept(NO_EXCEPT) : iterator_interface(ref, pos) {}

        inline value_type operator*() const noexcept(NO_EXCEPT) { return this->ref()->operator[](this->pos()); }
        inline value_type operator[](const typename iterator_interface::difference_type count) const noexcept(NO_EXCEPT) { return *(*this + count); }
    };

    inline iterator begin() const noexcept(NO_EXCEPT) { return iterator(this, 0); }
    inline iterator end() const noexcept(NO_EXCEPT) { return iterator(this, this->size()); }
};


} // namespace lib
