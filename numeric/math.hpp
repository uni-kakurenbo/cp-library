#pragma once

#include <vector>

#include "internal/dev_assert.hpp"
#include "snippet/iterations.hpp"

#include "numeric/modint.hpp"
#include "valarray.hpp"


namespace lib {


namespace binomial_lib {


template<class T>
struct small_binomial {
    using size_type = internal::size_t;
    using value_type = T;

  private:
    std::vector<lib::valarray<T>> _nPr, _nCr;

  public:
    constexpr small_binomial(const size_type max_n) : small_binomial(max_n, max_n) {};
    constexpr small_binomial(const size_type max_n, const size_type max_r) : _nPr(max_n+1, max_r+1), _nCr(max_n+1, max_r/2+1) {
        this->_nPr[0][0] = this->_nCr[0][0] = 1;
        FOR(i, 1, max_n) {
            this->_nPr[i][0] = this->_nCr[i][0] = 1;
            FOR(j, 1, max_r) {
                this->_nPr[i][j] = i * this->_nPr[i-1][j-1];
            }
            FOR(j, 1, max_r/2) {
                this->_nCr[i][j] = this->_nCr[i-1][j-1] + this->_nCr[i-1][j];
            }
        }
    }

    inline constexpr T nPr(const size_type n, size_type r) const {
        dev_assert(0 <= n);
        dev_assert(0 <= r);
        if(n < r) return 0;
        return this->_nPr[n][r];
    }

    inline constexpr T nCr(const size_type n, size_type r) const {
        dev_assert(0 <= n);
        dev_assert(0 <= r);
        if(n < r) return 0;
        if(n < r*2) r = n - r;
        return this->_nCr[n][r];
    }
};

template<class T>
struct small_binomial_prime_mod {
    static_assert(atcoder::internal::is_modint<T>::value);

    using size_type = internal::size_t;
    using value_type = T;

  private:
    lib::valarray<T> _inv, _fact, _fact_inv;

  public:
    small_binomial_prime_mod(const size_type max_n) : _inv(max_n+1), _fact(max_n+1), _fact_inv(max_n+1) {
        this->_fact[0] = this->_fact[1] = 1;
        this->_fact_inv[0] = this->_fact_inv[1] = 1;
        this->_inv[1] = 1;
        const auto mod = T::mod();
        FOR(i, 2, max_n) {
            this->_fact[i] = this->_fact[i-1] * i;
            this->_inv[i] =  -this->_inv[mod%i] * (mod/i);
            this->_fact_inv[i] = this->_fact_inv[i-1] * this->_inv[i];
        }
    }

    T nPr(size_type n, size_type r) const {
        dev_assert(0 <= n);
        dev_assert(0 <= r);
        if(n < r) return 0;
        return this->_fact[n] * this->_fact_inv[n-r];
    }

    T nCr(size_type n, size_type r) const {
        dev_assert(0 <= n);
        dev_assert(0 <= r);
        if(n < r) return 0;
        return this->_fact[n] * this->_fact_inv[r] * this->_fact_inv[n-r];
    }
};

template<class T, class N = std::int64_t>
struct large_binomial_prime_mod {
    static_assert(atcoder::internal::is_modint<T>::value);

    using size_type = internal::size_t;
    using value_type = T;

  private:
    lib::valarray<T> _inv, _fact_inv;

  public:
    large_binomial_prime_mod(const size_type max_r) : _inv(max_r/2+1), _fact_inv(max_r/2+1) {
        this->_fact_inv[0] = this->_fact_inv[1] = 1;
        this->_inv[1] = 1;
        const auto mod = T::mod();
        FOR(i, 2, max_r/2) {
            this->_inv[i] = -this->_inv[mod%i] * (mod/i);
            this->_fact_inv[i] = this->_fact_inv[i-1] * this->_inv[i];
        }
    }

    T nPr(size_type n, size_type r) const {
        dev_assert(0 <= n);
        dev_assert(0 <= r);
        if(n < r) return 0;
        T res = 1;
        FORD(i, n-r+1, n) res *= i;
        return res;
    }

    T nCr(const N n, size_type r) {
        if(n < r*2) r = n-r;
        return this->nPr(n, r) * this->_fact_inv[r];
    }
};

template<class T, class N = std::int64_t>
struct large_binomial_fixed_power_prime_mod {
    static_assert(atcoder::internal::is_modint<T>::value);

    using size_type = internal::size_t;
    using value_type = T;

  private:
    const N _n;
    lib::valarray<T> _nCr, _inv, _fact;

  public:
    inline N n() const { return this->_n; }


    large_binomial_fixed_power_prime_mod(const N n) : large_binomial_fixed_power_prime_mod(n, n) {}
    large_binomial_fixed_power_prime_mod(const N n, const size_type max_r)
      : _n(n), _nCr(max_r/2+1), _inv(max_r/2+1), _fact(max_r+1)
    {
        dev_assert(0 <= n);

        this->_fact[0] = this->_fact[1] = 1;
        this->_inv[1] = 1;
        const auto mod = T::mod();
        FOR(i, 2, max_r/2) {
            this->_inv[i] = -this->_inv[mod%i] * (mod/i);
        }
        FOR(i, 2, max_r) {
            this->_fact[i] = i * this->_fact[i-1];
        }
        this->_nCr[0] = 1;
        FOR(i, 1, max_r/2) {
            this->_nCr[i] = this->_nCr[i-1] * (n-i+1) * this->_inv[i];
        }
    }

    T nPr(size_type r) {
        return this->nCr(r) * this->_fact[r];
    }

    T nCr(size_type r) {
        dev_assert(0 <= r);
        if(this->_n < r) return 0;
        if(this->_n < r*2) r = this->_n-r;
        return this->_nCr[r];
    }

};


} // binomial_lib


using binomial_lib::small_binomial;
using binomial_lib::small_binomial_prime_mod;
using binomial_lib::large_binomial_prime_mod;
using binomial_lib::large_binomial_fixed_power_prime_mod;


} // namespace lib
