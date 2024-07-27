#pragma once


#include <cassert>
#include <algorithm>
#include <utility>
#include <ranges>

#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"
#include "internal/concepts.hpp"

#include "numeric/internal/mod.hpp"
#include "iterable/internal/operation_base.hpp"


#define UNI_ADVANCED_CONTAINER_OPERATOR(op_assign, op, concepts) \
    auto& operator op_assign(const value_type& v) noexcept(NO_EXCEPT) \
        requires concepts<value_type> \
    { \
        if constexpr(concepts<Base, value_type>) { \
            this->Base::operator op_assign(v); \
        } \
        else { \
            REP(itr, ALL(*this)) *itr op_assign v; \
        } \
        return *this; \
    } \
    \
    auto& operator op_assign(const advanced_container& rhs) noexcept(NO_EXCEPT) \
        requires concepts<value_type> \
    { \
        if constexpr(concepts<Base>) { \
            this->Base::operator op_assign(*rhs._base()); \
        } \
        else { \
            auto itr = std::ranges::begin(*this), rhs_itr = std::ranges::begin(rhs); \
            auto end = std::ranges::end(*this); \
            for(; itr != end; ++itr, ++rhs_itr) { \
                *itr op_assign *rhs_itr; \
            } \
        } \
        return *this; \
    } \
    \
    template<class T = value_type> \
        requires \
            concepts<value_type> && \
            (std::convertible_to<T, value_type> || std::same_as<T, advanced_container>) \
    friend auto operator op(advanced_container lhs, const T& rhs) noexcept(NO_EXCEPT) { \
        return lhs op_assign rhs; \
    } \
    \
    template<class T = value_type> \
        requires \
            concepts<value_type> && std::convertible_to<T, value_type> \
    friend auto operator op(const T& lhs, advanced_container rhs) noexcept(NO_EXCEPT) { \
        return advanced_container(rhs.size(), lhs) op_assign rhs; \
    }


namespace uni {

namespace internal {


template<class Base>
struct advanced_container : Base {
  private:
    inline Base* _base() noexcept(NO_EXCEPT) {
        return static_cast<Base*>(this);
    }
    inline const Base* _base() const noexcept(NO_EXCEPT) {
        return static_cast<const Base*>(this);
    }

  public:
    using Base::Base;

    advanced_container(const Base& base) : Base(base) {}

    using size_type = decltype(std::ranges::size(std::declval<Base>()));
    using value_type = Base::value_type;

    inline auto ssize() const noexcept(NO_EXCEPT) { return std::ranges::ssize(*this->_base()); }


    inline const auto& operator[](internal::size_t p) const noexcept(NO_EXCEPT) {
        p = p < 0 ? p + this->size() : p;
        assert(0 <= p && p < this->ssize());
        return this->Base::operator[](p);
    }

    inline auto& operator[](internal::size_t p) noexcept(NO_EXCEPT) {
        p = p < 0 ? p + this->size() : p;
        assert(0 <= p && p < this->ssize());
        return this->Base::operator[](p);
    }


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
    inline auto& sort(F&& f) noexcept(NO_EXCEPT) {
        std::ranges::sort(*this, std::forward<F>(f));
        return *this;
    }

    inline auto& stable_sort() noexcept(NO_EXCEPT) {
        std::ranges::stable_sort(*this);
        return *this;
    }

    template<class F>
    inline auto& stable_sort(F&& f) noexcept(NO_EXCEPT) {
        std::ranges::stable_sort(*this, std::forward<F>(f));
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
    inline auto count_if(F&& f) const noexcept(NO_EXCEPT) {
        return std::ranges::count_if(*this, std::forward<F>(f));
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
    inline auto& shuffle(F&& f) noexcept(NO_EXCEPT) {
        std::ranges::shuffle(*this, std::forward<F>(f));
        return *this;
    }

    inline auto& unique() noexcept(NO_EXCEPT) {
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

    inline auto join(const char* sep = "") noexcept(NO_EXCEPT) {
        return uni::join(*this, sep);
    }


    inline auto sum() const noexcept(NO_EXCEPT) { return uni::sum(*this); }


    inline auto max() const noexcept(NO_EXCEPT) { return std::ranges::max(*this->_base()); }
    inline auto min() const noexcept(NO_EXCEPT) { return std::ranges::min(*this); }


    inline auto begin() noexcept(NO_EXCEPT) { return std::ranges::begin(*this->_base()); }
    inline auto begin() const noexcept(NO_EXCEPT) { return std::ranges::begin(*this->_base()); }

    inline auto end() noexcept(NO_EXCEPT) { return std::ranges::end(*this->_base()); }
    inline auto end() const noexcept(NO_EXCEPT) { return std::ranges::end(*this->_base()); }


    UNI_ADVANCED_CONTAINER_OPERATOR(+=, +, internal::weakly_addition_assignable)
    UNI_ADVANCED_CONTAINER_OPERATOR(-=, -, internal::weakly_subtraction_assignable)
    UNI_ADVANCED_CONTAINER_OPERATOR(*=, *, internal::weakly_multipliation_assignalbe)
    UNI_ADVANCED_CONTAINER_OPERATOR(/=, /, internal::weakly_division_assignable)
    UNI_ADVANCED_CONTAINER_OPERATOR(%=, %, internal::weakly_remainder_assignable)
    UNI_ADVANCED_CONTAINER_OPERATOR(&=, &, internal::weakly_bitand_assignable)
    UNI_ADVANCED_CONTAINER_OPERATOR(|=, |, internal::weakly_bitor_assignable)
    UNI_ADVANCED_CONTAINER_OPERATOR(^=, ^, internal::weakly_bitxor_assignable)
};


} // namespace internal

} // namespace uni

#undef UNI_ADVANCED_CONTAINER_OPERATOR
