#pragma once


#include <cassert>
#include <algorithm>
#include <utility>
#include <ranges>


#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "numeric/internal/mod.hpp"
#include "iterable/internal/operation_base.hpp"


namespace lib {

namespace internal {


template<class Base>
struct extended_container : Base {
  private:
    inline Base* _base() noexcept(NO_EXCEPT) {
        return static_cast<Base*>(this);
    }
    inline const Base* _base() const noexcept(NO_EXCEPT) {
        return static_cast<const Base*>(this);
    }

  public:
    using Base::Base;

    extended_container(const Base& base) : Base(base) {}

    using size_type = typename Base::size_type;
    using value_type = typename Base::value_type;

    inline auto ssize() const noexcept(NO_EXCEPT) { return std::ranges::ssize(*this); }


    inline const auto& operator[](internal::size_t p) const noexcept(NO_EXCEPT) {
        p = p < 0 ? p + this->size() : p;
        assert(0 <= p && p < static_cast<internal::size_t>(this->size()));
        return this->Base::operator[](p);
    }

    inline auto& operator[](internal::size_t p) noexcept(NO_EXCEPT) {
        p = p < 0 ? p + this->size() : p;
        assert(0 <= p && p < static_cast<internal::size_t>(this->size()));
        return this->Base::operator[](p);
    }


    inline auto& fill(const value_type& v) noexcept(NO_EXCEPT) {
        std::ranges::fill(*this->_base(), v);
        return *this;
    }

    inline auto& swap(const size_type i, const size_type j) noexcept(NO_EXCEPT) {
        std::swap(this->operator[](i), this->operator[](j));
        return *this;
    }

    inline auto& sort() noexcept(NO_EXCEPT) {
        std::ranges::sort(*this->_base());
        return *this;
    }

    template<class F>
    inline auto& sort(const F& f) noexcept(NO_EXCEPT) {
        std::ranges::sort(*this->_base(), f);
        return *this;
    }

    inline auto& stable_sort() noexcept(NO_EXCEPT) {
        std::ranges::stable_sort(*this->_base());
        return *this;
    }

    template<class F>
    inline auto& stable_sort(const F& f) noexcept(NO_EXCEPT) {
        std::ranges::stable_sort(*this->_base(), f);
        return *this;
    }

    inline auto& reverse() noexcept(NO_EXCEPT) {
        std::ranges::reverse(*this->_base());
        return *this;
    }

    inline auto count(const value_type& v) const noexcept(NO_EXCEPT) {
        return std::ranges::count(*this->_base(), v);
    }

    template<class F>
    inline auto count_if(F& f) const noexcept(NO_EXCEPT) {
        return std::ranges::count_if(*this->_base(), f);
    }

    inline auto& resize(const size_type k) noexcept(NO_EXCEPT) {
        this->Base::resize(k);
        return *this;
    }
    inline auto& resize(const size_type k, const value_type v) noexcept(NO_EXCEPT) {
        this->Base::resize(k, v);
        return *this;
    }

    template<class F>
    inline auto& shuffle(const F& f) noexcept(NO_EXCEPT) {
        std::ranges::shuffle(*this->_base(), f);
        return *this;
    }

    inline auto& unique() noexcept(NO_EXCEPT) {
        std::ranges::sort(*this->_base());
        const auto rest = std::ranges::unique(*this->_base());
        this->erase(ALL(rest));
        return *this;
    }

    template<class T>
    inline auto binary_search(const T& v) noexcept(NO_EXCEPT) {
        return std::ranges::binary_search(*this->_base(), v);
    }

    template<class T>
    inline auto lower_bound(const T& v) noexcept(NO_EXCEPT) {
        return std::ranges::lower_bound(*this->_base(), v);
    }

    template<class T>
    inline auto upper_bound(const T& v) noexcept(NO_EXCEPT) {
        return std::ranges::upper_bound(*this->_base(), v);
    }

    inline auto join(const char* sep = "") noexcept(NO_EXCEPT) {
        return lib::join(*this->_base(), sep);
    }
};


} // namespace internal

} // namespace lib
