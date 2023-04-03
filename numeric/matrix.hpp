#pragma once

#include <vector>

#include "internal/dev_assert.hpp"
#include "grid.hpp"
#include "valarray.hpp"

namespace lib {

namespace internal {

namespace matrix_lib {

template<class T> struct interface : virtual grid_lib::interface<T> {
    virtual size_t rows() const = 0;
    virtual size_t cols() const = 0;

    virtual size_t square() const = 0;
};

}

template<class T, class base>
struct matrix_core : base, virtual matrix_lib::interface<T> {
    using base::base;

    static inline matrix_core identity(const size_t n, const T &&val = { 1 }) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }

    inline size_t rows() const override { return this->height(); }
    inline size_t cols() const override { return this->width(); }

    inline size_t square() const override { return this->rows() == this->cols(); }

    template<class U> inline matrix_core& operator+=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template<class ...U> inline matrix_core& operator+=(const matrix_core<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template<class U> inline matrix_core operator+(const U rhs) const {
        return matrix_core(*this) += rhs;
    }

    template<class U> inline matrix_core& operator-=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template<class ...U> inline matrix_core& operator-=(const matrix_core<U...> rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template<class U> inline matrix_core operator-(const U rhs) const {
        return matrix_core(*this) -= rhs;
    }

    template<class ...U> inline matrix_core operator*(const matrix_core<U...> rhs) {
        dev_assert(this->cols() == rhs.rows());
        matrix_core res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template<class U> inline matrix_core operator*(const U rhs) {
        matrix_core res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template<class U> inline matrix_core& operator*=(const U rhs) {
        matrix_core res = *this * rhs;
        this->assign(res);
        return *this;
    }

    template<class U> inline matrix_core& operator/=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template<class U> inline matrix_core operator/(const U rhs) const {
        return matrix_core(*this) /= rhs;
    }

    template<class U> inline matrix_core& operator%=(const U rhs) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template<class U> inline matrix_core operator%(const U rhs) const {
        return matrix_core(*this) %= rhs;
    }

    inline matrix_core pow(ll p) {
        dev_assert(this->square());
        matrix_core x = *this, res = matrix_core::Identity(this->rows());
        while(p > 0) {
            if(p & 1) res *= x;
            x *= x;
            p >>= 1;
        }
        return res;
    }
};

} // namespace internal

template<class T, class base = grid<T>>
using matrix = internal::matrix_core<T,base>;

template<class T>
using valmatrix = internal::matrix_core<T,unfolded_grid<T,valarray<T>>>;

template<class T>
using unfolded_matrix = internal::matrix_core<T,unfolded_grid<T>>;

} // namespace lib
