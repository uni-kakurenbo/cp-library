#pragma once


#include <cassert>
#include <vector>

#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"

#include "grid.hpp"
#include "adapter/valarray.hpp"


namespace lib {

namespace internal {

namespace matrix_impl {

template<class T> struct interface : virtual grid_impl::interface<T> {
    // virtual size_t rows() const noexcept(DEV_ENV)= 0;
    // virtual size_t cols() const noexcept(DEV_ENV)= 0;

    // virtual size_t square() const noexcept(DEV_ENV)= 0;
};

}

template<class T, class base>
struct matrix_core : base, virtual matrix_impl::interface<T> {
    using base::base;

    static inline matrix_core identity(const size_t n, const T &&val = { 1 }) noexcept(DEV_ENV) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }

    inline size_t rows() const noexcept(DEV_ENV) /*override*/ { return this->height(); }
    inline size_t cols() const noexcept(DEV_ENV) /*override*/ { return this->width(); }

    inline size_t square() const noexcept(DEV_ENV) /*override*/ { return this->rows() == this->cols(); }

    template<class U> inline matrix_core& operator+=(const U rhs) noexcept(DEV_ENV) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template<class ...U> inline matrix_core& operator+=(const matrix_core<U...> rhs) noexcept(DEV_ENV) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template<class U> inline matrix_core operator+(const U rhs) const noexcept(DEV_ENV) {
        return matrix_core(*this) += rhs;
    }

    template<class U> inline matrix_core& operator-=(const U rhs) noexcept(DEV_ENV) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template<class ...U> inline matrix_core& operator-=(const matrix_core<U...> rhs) noexcept(DEV_ENV) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template<class U> inline matrix_core operator-(const U rhs) const noexcept(DEV_ENV) {
        return matrix_core(*this) -= rhs;
    }

    template<class ...U> inline matrix_core operator*(const matrix_core<U...> rhs) noexcept(DEV_ENV) {
        assert(this->cols() == rhs.rows());
        matrix_core res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template<class U> inline matrix_core operator*(const U rhs) noexcept(DEV_ENV) {
        matrix_core res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template<class U> inline matrix_core& operator*=(const U rhs) noexcept(DEV_ENV) {
        matrix_core res = *this * rhs;
        this->assign(res);
        return *this;
    }

    template<class U> inline matrix_core& operator/=(const U rhs) noexcept(DEV_ENV) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template<class U> inline matrix_core operator/(const U rhs) const noexcept(DEV_ENV) {
        return matrix_core(*this) /= rhs;
    }

    template<class U> inline matrix_core& operator%=(const U rhs) noexcept(DEV_ENV) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template<class U> inline matrix_core operator%(const U rhs) const noexcept(DEV_ENV) {
        return matrix_core(*this) %= rhs;
    }

    inline matrix_core pow(ll p) noexcept(DEV_ENV) {
        assert(this->square());
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
