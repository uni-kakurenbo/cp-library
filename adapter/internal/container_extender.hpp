#pragma once


#include <algorithm>
#include <utility>

#include "internal/dev_env.hpp"


namespace lib {

namespace internal {


template<class Base>
struct extended_container : Base {
    using Base::Base;

    extended_container(const Base& base) : Base(base) {}

    using size_type = decltype(Base().size());
    using value_type = typename Base::value_type;

    // inline auto ssize() const noexcept(NO_EXCEPT) { return to_signed(this->size()); }

    inline auto& fill(const value_type& v) noexcept(NO_EXCEPT) {
        std::ranges::fill(*this, v);
        return *this;
    }

    inline auto& swap(const size_type i, const size_type j) noexcept(NO_EXCEPT) {
        std::swap(this->operator[](i), this->operator[](j));
        return *this;
    }

    inline auto& sort() noexcept(NO_EXCEPT) {
        std::ranges::sort(*this);
        return *this;
    }

    template<class F>
    inline auto& sort(const F& f) noexcept(NO_EXCEPT) {
        std::ranges::sort(*this, f);
        return *this;
    }

    inline auto& stable_sort() noexcept(NO_EXCEPT) {
        std::ranges::stable_sort(*this);
        return *this;
    }

    template<class F>
    inline auto& stable_sort(const F& f) noexcept(NO_EXCEPT) {
        std::ranges::stable_sort(*this, f);
        return *this;
    }

    inline auto& reverse() noexcept(NO_EXCEPT) {
        std::ranges::reverse(*this);
        return *this;
    }

    inline auto count(const value_type& v) const noexcept(NO_EXCEPT) {
        return std::ranges::count(*this, v);
    }

    template<class F>
    inline auto count_if(F& f) const noexcept(NO_EXCEPT) {
        return std::ranges::count_if(*this, f);
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
        std::ranges::shuffle(*this, f);
        return *this;
    }

    inline auto& unique() noexcept(NO_EXCEPT) {
        std::ranges::sort(*this);
        const auto rest = std::ranges::unique(*this);
        this->erase(ALL(rest));
        return *this;
    }

    template<class T>
    inline auto binary_search(const T& v) noexcept(NO_EXCEPT) {
        return std::ranges::binary_search(*this, v);
    }

    template<class T>
    inline auto lower_bound(const T& v) noexcept(NO_EXCEPT) {
        return std::ranges::lower_bound(*this, v);
    }

    template<class T>
    inline auto upper_bound(const T& v) noexcept(NO_EXCEPT) {
        return std::ranges::upper_bound(*this, v);
    }
};


} // namespace internal

} // namespace lib
