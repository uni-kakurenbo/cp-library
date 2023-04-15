#pragma once

#include <utility>

#include "numeric/arithmetic.hpp"


namespace lib {


template<class T, T INF, T SUP> struct static_restrictor {
    using restrictor = static_restrictor;

  protected:
    T _v;

    inline void _clamp() { this->_v = std::clamp(this->_v, INF, SUP); }
    inline restrictor& _assign(T v) {
        this->_v = std::clamp(v, INF, SUP);
        return *this;
    }

  public:
    static_restrictor(T v = T{}) : _v(v) {}
    inline T val() const { return this->_v; }

    restrictor& operator++() { return this->_assign(this->_v + 1); }
    restrictor& operator--() { return this->_assign(this->_v - 1); }
    restrictor operator++(int) {
        restrictor res = *this;
        *this++;
        return res;
    }
    restrictor operator--(int) {
        restrictor res = *this;
        *this--;
        return res;
    }

    restrictor& operator+=(const restrictor& rhs) { return this->_assign(this->_v + rhs.val()); }
    restrictor& operator-=(const restrictor& rhs) { return this->_assign(this->_v - rhs.val()); }
    restrictor& operator*=(const restrictor& rhs) {
        if(mul_over(this->_v, rhs.val(), SUP)) return this->_assign(SUP);
        if(mul_under(this->_v, rhs.val(), INF)) return this->_assign(INF);
        return this->_assign(this->_v * rhs.val());
    }
    restrictor& operator/=(const restrictor& rhs) { return this->_assign(this->_v / rhs.val()); }

    restrictor operator+() const { return *this; }
    restrictor operator-() const { return restrictor() - *this; }

    friend restrictor operator+(const restrictor& lhs, const restrictor& rhs) {
        return restrictor(lhs) += rhs;
    }
    friend restrictor operator-(const restrictor& lhs, const restrictor& rhs) {
        return restrictor(lhs) -= rhs;
    }
    friend restrictor operator*(const restrictor& lhs, const restrictor& rhs) {
        return restrictor(lhs) *= rhs;
    }
    friend restrictor operator/(const restrictor& lhs, const restrictor& rhs) {
        return restrictor(lhs) /= rhs;
    }
    friend bool operator==(const restrictor& lhs, const restrictor& rhs) {
        return lhs._v == rhs._v;
    }
    friend bool operator!=(const restrictor& lhs, const restrictor& rhs) {
        return lhs._v != rhs._v;
    }
    friend bool operator<(const restrictor& lhs, const restrictor& rhs) {
        return lhs._v < rhs._v;
    }
    friend bool operator>(const restrictor& lhs, const restrictor& rhs) {
        return lhs._v > rhs._v;
    }
    friend bool operator<=(const restrictor& lhs, const restrictor& rhs) {
        return lhs._v <= rhs._v;
    }
    friend bool operator>=(const restrictor& lhs, const restrictor& rhs) {
        return lhs._v >= rhs._v;
    }
};


} // namespace lib


namespace std {
    template<class T, T INF, T SUP>
    T std::abs(const lib::static_restrictor<T,INF,SUP>& v) { return std::abs(v.val()); }
}
