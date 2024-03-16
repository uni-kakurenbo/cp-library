#pragma once


#include <cassert>
#include <vector>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"

#include "structure/grid.hpp"
#include "adaptor/valarray.hpp"

#include "numeric/modular/modint_interface.hpp"



namespace lib {

namespace internal {

namespace matrix_impl {

template<class T> struct interface : virtual grid_impl::interface<T> {
    // virtual size_t rows() const noexcept(NO_EXCEPT)= 0;
    // virtual size_t cols() const noexcept(NO_EXCEPT)= 0;

    // virtual size_t square() const noexcept(NO_EXCEPT)= 0;
};

}

template<class T, class base>
struct matrix_core : base, virtual matrix_impl::interface<T> {
    using base::base;

    static inline matrix_core identity(const size_t n, const T &&val = { 1 }) noexcept(NO_EXCEPT) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }

    inline size_t rows() const noexcept(NO_EXCEPT) /*override*/ { return this->height(); }
    inline size_t cols() const noexcept(NO_EXCEPT) /*override*/ { return this->width(); }

    inline size_t square() const noexcept(NO_EXCEPT) /*override*/ { return this->rows() == this->cols(); }

    template<class U> inline matrix_core& operator+=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs;
        return *this;
    }
    template<class ...U> inline matrix_core& operator+=(const matrix_core<U...> rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) += rhs(i, j);
        return *this;
    }
    template<class U> inline matrix_core operator+(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) += rhs;
    }

    template<class U> inline matrix_core& operator-=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs;
        return *this;
    }
    template<class ...U> inline matrix_core& operator-=(const matrix_core<U...> rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) -= rhs(i, j);
        return *this;
    }
    template<class U> inline matrix_core operator-(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) -= rhs;
    }

    template<class ...U> inline matrix_core operator*(const matrix_core<U...> rhs) noexcept(NO_EXCEPT) {
        assert(this->cols() == rhs.rows());
        matrix_core res(this->rows(), rhs.cols());
        REP(i, this->rows()) REP(j, rhs.cols()) REP(k, this->cols()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }
    template<class U> inline matrix_core operator*(const U rhs) noexcept(NO_EXCEPT) {
        matrix_core res(*this);
        REP(i, res.rows()) REP(j, res.cols()) res(i, j) *= rhs;
        return res;
    }
    template<class U> inline matrix_core& operator*=(const U rhs) noexcept(NO_EXCEPT) {
        matrix_core res = *this * rhs;
        this->assign(res);
        return *this;
    }

    template<class U> inline matrix_core& operator/=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) /= rhs;
        return *this;
    }
    template<class U> inline matrix_core operator/(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) /= rhs;
    }

    template<class U> inline matrix_core& operator%=(const U rhs) noexcept(NO_EXCEPT) {
        REP(i, this->rows()) REP(j, this->cols()) (*this)(i, j) %= rhs;
        return *this;
    }
    template<class U> inline matrix_core operator%(const U rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) %= rhs;
    }

    inline matrix_core pow(ll p) noexcept(NO_EXCEPT) {
        assert(this->square());
        matrix_core x = *this, res = matrix_core::Identity(this->rows());
        while(p > 0) {
            if(p & 1) res *= x;
            x *= x;
            p >>= 1;
        }
        return res;
    }

    T determinant() const noexcept(NO_EXCEPT)
        requires modint_family<T>
    {
        assert(this->rows() == this->cols());

        auto a = *this;

        T det = T::one;
        REP(j, a.rows()) {
            REP(i, j, a.rows()) {
                if(a[i][j] == 0) continue;
                if(i != j) std::swap(a[i], a[j]), det = -det;
                break;
            }

            if(a[j][j] == 0) return 0;
            REP(i, j+1, a.rows()) {
                while(a[i][j] != 0) {
                    const long long q = a[j][j].val() / a[i][j].val();
                    const T c = -q;
                    REP(k, j, a.rows()) a[j][k] += a[i][k] * c;
                    std::swap(a[i], a[j]), det = -det;
                }
            }
        }

        REP(i, a.rows()) det *= a[i][i];
        return det;
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
