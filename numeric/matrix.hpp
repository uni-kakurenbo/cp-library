#pragma once


#include <cassert>
#include <vector>


#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/concepts.hpp"

#include "structure/grid.hpp"

#include "numeric/modular/modint_interface.hpp"



namespace uni {

namespace internal {


template<class Base>
struct matrix_core : Base {
    using Base::Base;

    using value_type = typename Base::value_type;
    using size_type = typename Base::size_type;

    using vector_type = typename Base::row_type;

    static constexpr auto identity(const size_type n, const value_type& val = 1) noexcept(NO_EXCEPT) {
        matrix_core res(n);
        REP(i, n) res(i, i) = val;
        return res;
    }

    inline constexpr bool square() const noexcept(NO_EXCEPT) { return this->height() == this->width(); }


    constexpr auto& operator+=(const value_type& rhs) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) this->operator()(i, j) += rhs;
        return *this;
    }

    template<class... Ts>
    constexpr auto& operator+=(const matrix_core<Ts...>& rhs) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) this->operator()(i, j) += rhs(i, j);
        return *this;
    }


    template<weakly_addition_assignable<matrix_core> T>
    inline constexpr auto operator+(const T& rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) += rhs;
    }


    constexpr auto& operator-=(const value_type& rhs) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) this->operator()(i, j) -= rhs;
        return *this;
    }

    template<class... Ts>
    constexpr auto& operator-=(const matrix_core<Ts...>& rhs) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) this->operator()(i, j) -= rhs(i, j);
        return *this;
    }


    template<weakly_subtraction_assignable<matrix_core> T>
    inline constexpr auto operator-(const T& rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) -= rhs;
    }


    template<class... Ts>
    constexpr auto operator*(const matrix_core<Ts...>& rhs) noexcept(NO_EXCEPT) {
        assert(this->width() == rhs.height());
        matrix_core res(this->height(), rhs.width());
        REP(i, this->height()) REP(j, rhs.width()) REP(k, this->width()) {
            res(i, j) += (*this)(i, k) * rhs(k, j);
        }
        return res;
    }

    template<std::ranges::input_range Vector>
    constexpr auto operator*(const Vector& rhs) noexcept(NO_EXCEPT) {
        assert(this->width() == std::ranges::ssize(rhs));
        Vector res(this->height());
        REP(i, this->height()) REP(j, this->width()) res[i] += this->operator()(i, j) * rhs[j];
        return res;
    }

    template<std::ranges::input_range Vector>
    friend constexpr auto operator*(const Vector& lhs, const matrix_core& rhs) noexcept(NO_EXCEPT) {
        assert(std::ranges::ssize(lhs) == rhs.height());
        Vector res(rhs.width());
        REP(i, rhs.height()) REP(j, rhs.width()) res[j] += lhs[j] * rhs[i][j];
        return res;
    }

    constexpr auto operator*(const value_type& rhs) noexcept(NO_EXCEPT) {
        auto res = *this;
        REP(i, res.height()) REP(j, res.width()) res(i, j) *= rhs;
        return res;
    }


    template<multipliable<matrix_core> T>
    constexpr auto& operator*=(const T& rhs) noexcept(NO_EXCEPT) {
        matrix_core res = *this * rhs;
        this->assign(res);
        return *this;
    }

    constexpr auto& operator/=(const value_type& rhs) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) this->operator()(i, j) /= rhs;
        return *this;
    }

    inline constexpr auto operator/(const value_type& rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) /= rhs;
    }

    constexpr auto& operator%=(const value_type& rhs) noexcept(NO_EXCEPT) {
        REP(i, this->height()) REP(j, this->width()) this->operator()(i, j) %= rhs;
        return *this;
    }

    inline constexpr auto operator%(const value_type& rhs) const noexcept(NO_EXCEPT) {
        return matrix_core(*this) %= rhs;
    }


    constexpr auto find_pivot(const size_type rank, const size_type col) const noexcept(NO_EXCEPT) {
        size_type pivot = -1;

        REP(i, rank, this->height()) {
            if(this->operator()(i, col) != 0) {
                pivot = i;
                break;
            }
        }

        return pivot;
    }

    template<bool DIAGONALIZE = false>
    constexpr void sweep(const size_type rank, const size_type col, const size_type pivot) noexcept(NO_EXCEPT)
        requires modint_family<value_type>
    {
        std::swap(this->operator[](pivot), this->operator[](rank));

        if constexpr(DIAGONALIZE) {
            const auto inv = this->operator()(rank, col).inv();
            REP(j, this->width()) this->operator()(rank, j) *= inv;
        }

        REP(i, (DIAGONALIZE ? 0 : rank + 1), this->height()) {
            if(i != rank && this->operator()(i, col) != 0) {
                const auto fac = this->operator()(i, col);
                REP(j, this->width()) this->operator()(i, j) -= this->operator()(rank, j) * fac;
            }
        }
    }


    template<size_type IGNORE_WIDTH = 0, bool DIAGONALIZE = false>
    inline constexpr auto transform_to_upper_triangular() noexcept(NO_EXCEPT)
        requires modint_family<value_type>
    {
        size_type rank = 0;

        REP(j, this->width() - IGNORE_WIDTH) {
            const auto pivot = this->find_pivot(rank, j);
            if(pivot == -1) continue;
            this->template sweep<DIAGONALIZE>(rank++, j, pivot);
        }

        return rank;
    }

    template<size_type IGNORE_WIDTH>
    inline constexpr auto transform_to_lower_triangular() noexcept(NO_EXCEPT)
        requires modint_family<value_type>
    {
        const auto rank = this->template transform_to_upper_triangular<IGNORE_WIDTH>();
        this->transpose();
        return rank;
    }


    template<std::ranges::input_range Vector, class Sol = void, class System = void>
    constexpr std::optional<size_type> solve_linear_equation(
        const Vector& v,
        Sol *const sol = nullptr,
        System *const system = nullptr
    )
        const noexcept(NO_EXCEPT)
        requires modint_family<value_type>
    {
        static_assert(!(std::same_as<Sol, void>) || (std::same_as<System, void>));

        assert(this->height() == std::ranges::ssize(v));

        matrix_core mat(this->height(), this->width() + 1);

        REP(i, this->height()) {
            REP(j, this->width()) mat(i, j) = this->operator()(i, j);
            mat(i, this->width()) = v[i];
        }

        const auto rank = mat.transform_to_upper_triangular<1, true>();

        REP(i, rank, this->height()) if(mat(i, this->width()) != value_type::zero) return {};

        if constexpr(!std::same_as<Sol, void>) {
            static_assert(std::ranges::output_range<Sol, value_type>);
            static_assert(std::ranges::output_range<std::ranges::range_value_t<System>, value_type>);
            assert(!sol || system);

            if(sol) {
                sol->assign(this->width(), value_type::zero);

                std::vector<size_type> pivots(system ? this->width() : 0, -1);

                for(size_type i = 0, j = 0; i < rank; ++i) {
                    while(mat(i, j) == value_type::zero) ++j;

                    sol->operator[](j) = mat(i, this->width());

                    if constexpr(!std::same_as<System, void>) if(system) {
                        pivots[j] = i;
                    }
                }

                if constexpr(!std::same_as<System, void>) if(system) {
                    REP(j, this->width()) {
                        if(pivots[j] != -1) continue;

                        typename System::vector_type x(this->width());
                        x[j] = value_type::one;
                        REP(k, j) if(pivots[k] != -1) x[k] = -mat(pivots[k], j);
                        system->push_back(x);
                    }

                    system->resize(system->size(), this->width());
                }
            }
        }

        return rank;
    }

    value_type determinant() const noexcept(NO_EXCEPT)
        requires modint_family<value_type>
    {
        assert(this->square());

        auto mat = *this;

        value_type det = value_type::one;

        REP(j, this->height()) {
            const auto pivot = mat.find_pivot(j, j);
            if(j < pivot) std::swap(mat[pivot], mat[j]), det = -det;

            if(mat(j, j) == 0) return 0;

            REP(i, j + 1, this->height()) {
                while(mat(i, j) != 0) {
                    const auto c = static_cast<value_type>(-to_signed(mat(j, j).val() / mat(i, j).val()));
                    REP(k, j, this->height()) mat(j, k) += mat(i, k) * c;
                    std::swap(mat[i], mat[j]), det = -det;
                }
            }
        }

        REP(i, mat.height()) det *= mat(i, i);

        return det;
    }
};


} // namespace internal


template<class T, class Base = grid<T>>
using matrix = internal::matrix_core<Base>;

template<class T>
using valmatrix = internal::matrix_core<valgrid<T>>;

template<class T>
using unfolded_matrix = internal::matrix_core<unfolded_grid<T>>;

template<class T>
using unfolded_valmatrix = internal::matrix_core<unfolded_valgrid<T>>;


} // namespace uni
