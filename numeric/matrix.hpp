#pragma once

#include <vector>

#include "develop/dev_assert.hpp"
#include "grid.hpp"


namespace Internal {

namespace MatrixLib {

template<class T> struct Interface : virtual GridLib::Interface<T> {
    virtual int rows() const = 0;
    virtual int cols() const = 0;

    virtual int square() const = 0;
};

}

template<class T, class Base>
struct Matrix : Base, virtual MatrixLib::Interface<T> {
    using Base::Base;

    static inline Matrix Identity(const int n, const T &&val = { 1 }) {
        Matrix res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }

    inline int rows() const override { return this->height(); }
    inline int cols() const override { return this->width(); }

    inline int square() const override { return this->rows() == this->cols(); }

    template<class U> inline Matrix& operator+=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template<class ...U> inline Matrix& operator+=(const Matrix<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template<class U> inline Matrix operator+(const U rhs) const {
        return Matrix(*this) += rhs;
    }

    template<class U> inline Matrix& operator-=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template<class ...U> inline Matrix& operator-=(const Matrix<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template<class U> inline Matrix operator-(const U rhs) const {
        return Matrix(*this) -= rhs;
    }

    template<class ...U> inline Matrix operator*(const Matrix<U...> rhs) {
        dev_assert(this->cols() == rhs.rows());
        Matrix res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template<class U> inline Matrix operator*(const U rhs) {
        Matrix res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template<class U> inline Matrix& operator*=(const U rhs) {
        Matrix res = *this * rhs;
        this->assign(res);
        return *this;
    }

    template<class U> inline Matrix& operator/=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template<class U> inline Matrix operator/(const U rhs) const {
        return Matrix(*this) /= rhs;
    }

    template<class U> inline Matrix& operator%=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template<class U> inline Matrix operator%(const U rhs) const {
        return Matrix(*this) %= rhs;
    }

    inline Matrix pow(ll p) {
        dev_assert(this->square());
        Matrix x = *this, res = Matrix::Identity(this->rows());
        while(p > 0) {
            if(p & 1) res *= x;
            x *= x;
            p >>= 1;
        }
        return res;
    }

    inline T sum() {
        T res = T{};
        REP(i, this->rows()) REP(j, this->cols()) res += (*this)(i, j);
        return res;
    }
};

} // namespace Internal

template<class T, class Base = Grid<T>>
using Matrix = Internal::Matrix<T,Base>;
