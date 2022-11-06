#pragma once

#include <vector>

#include "internal/develop/dev_assert.hpp"
#include "grid.hpp"
#include "val_array.hpp"

namespace Lib {

namespace Internal {

namespace MatrixLib {

template<class T> struct Interface : virtual GridLib::Interface<T> {
    virtual Size rows() const = 0;
    virtual Size cols() const = 0;

    virtual Size square() const = 0;
};

}

template<class T, class Base>
struct MatrixCore : Base, virtual MatrixLib::Interface<T> {
    using Base::Base;

    static inline MatrixCore Identity(const Size n, const T &&val = { 1 }) {
        MatrixCore res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }

    inline Size rows() const override { return this->height(); }
    inline Size cols() const override { return this->width(); }

    inline Size square() const override { return this->rows() == this->cols(); }

    template<class U> inline MatrixCore& operator+=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template<class ...U> inline MatrixCore& operator+=(const MatrixCore<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template<class U> inline MatrixCore operator+(const U rhs) const {
        return MatrixCore(*this) += rhs;
    }

    template<class U> inline MatrixCore& operator-=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template<class ...U> inline MatrixCore& operator-=(const MatrixCore<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template<class U> inline MatrixCore operator-(const U rhs) const {
        return MatrixCore(*this) -= rhs;
    }

    template<class ...U> inline MatrixCore operator*(const MatrixCore<U...> rhs) {
        dev_assert(this->cols() == rhs.rows());
        MatrixCore res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template<class U> inline MatrixCore operator*(const U rhs) {
        MatrixCore res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template<class U> inline MatrixCore& operator*=(const U rhs) {
        MatrixCore res = *this * rhs;
        this->assign(res);
        return *this;
    }

    template<class U> inline MatrixCore& operator/=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template<class U> inline MatrixCore operator/(const U rhs) const {
        return MatrixCore(*this) /= rhs;
    }

    template<class U> inline MatrixCore& operator%=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template<class U> inline MatrixCore operator%(const U rhs) const {
        return MatrixCore(*this) %= rhs;
    }

    inline MatrixCore pow(ll p) {
        dev_assert(this->square());
        MatrixCore x = *this, res = MatrixCore::Identity(this->rows());
        while(p > 0) {
            if(p & 1) res *= x;
            x *= x;
            p >>= 1;
        }
        return res;
    }
};

} // namespace Internal

template<class T, class Base = Grid<T>>
using Matrix = Internal::MatrixCore<T,Base>;

template<class T>
using ValMatrix = Internal::MatrixCore<T,UnfoldedGrid<T,ValArray<T>>>;

template<class T>
using UnfoldedMatrix = Internal::MatrixCore<T,UnfoldedGrid<T>>;

} // namespace Lib
