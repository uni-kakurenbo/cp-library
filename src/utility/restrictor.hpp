#pragma once

#include <utility>

#include "numeric/arithmetic.hpp"


namespace lib {


template<class T, T INF, T SUP>
struct static_restrictor {
    using restrictor = static_restrictor;

  protected:
    T _v = INF;

    inline void _clamp() noexcept(NO_EXCEPT) { this->_v = std::clamp(this->_v, INF, SUP); }
    inline restrictor& _assign(const T& v) noexcept(NO_EXCEPT) {
        this->_v = std::clamp(v, INF, SUP);
        return *this;
    }
    inline restrictor& _assign_raw(const T& v) noexcept(NO_EXCEPT) {
        this->_v = v;
        return *this;
    }

  public:
    static_restrictor() noexcept(NO_EXCEPT) = default;
    static_restrictor(T v) noexcept(NO_EXCEPT) : _v(v) { this->_clamp(); }

    inline T val() const noexcept(NO_EXCEPT) { return this->_v; }

    static inline static_restrictor raw(const T& v) noexcept(NO_EXCEPT) {
        static_restrictor res;
        res._assign_raw(v);
        return res;
    }

    restrictor& operator++() noexcept(NO_EXCEPT) { return this->_assign(this->_v + 1); }
    restrictor& operator--() noexcept(NO_EXCEPT) { return this->_assign(this->_v - 1); }
    restrictor operator++(int) noexcept(NO_EXCEPT) { auto res = *this; return ++(*this), res; }
    restrictor operator--(int) noexcept(NO_EXCEPT) { auto res = *this; return --(*this), res; }

    restrictor& operator+=(const restrictor& rhs) noexcept(NO_EXCEPT) {
        return this->_assign_raw(add_clamp(this->_v, rhs._v, INF, SUP));
    }
    restrictor& operator-=(const restrictor& rhs) noexcept(NO_EXCEPT) {
        return this->_assign_raw(sub_clamp(this->_v, rhs._v, INF, SUP));
    }
    restrictor& operator*=(const restrictor& rhs) noexcept(NO_EXCEPT) {
        return this->_assign_raw(mul_clamp(this->_v, rhs._v, INF, SUP));
    }
    restrictor& operator/=(const restrictor& rhs) noexcept(NO_EXCEPT) {
        return this->_assign(this->_v / rhs.val());
    }

    restrictor operator+() const noexcept(NO_EXCEPT) { return *this; }
    restrictor operator-() const noexcept(NO_EXCEPT) { return restrictor(-this->_v); }

    friend restrictor operator+(restrictor lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs += rhs;
    }
    friend restrictor operator-(restrictor lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs -= rhs;
    }
    friend restrictor operator*(restrictor lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs *= rhs;
    }
    friend restrictor operator/(restrictor lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs /= rhs;
    }
    friend bool operator==(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v != rhs._v;
    }
    friend bool operator<(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v < rhs._v;
    }
    friend bool operator>(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v > rhs._v;
    }
    friend bool operator<=(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v <= rhs._v;
    }
    friend bool operator>=(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v >= rhs._v;
    }
    friend bool operator<=>(const restrictor& lhs, const restrictor& rhs) noexcept(NO_EXCEPT) {
        return lhs._v <=> rhs._v;
    }
};


} // namespace lib


namespace std {


template<class T, T INF, T SUP>
T abs(const lib::static_restrictor<T,INF,SUP>& v) noexcept(NO_EXCEPT) { return std::abs(v.val()); }


}
