#pragma once

#include <cassert>
#include <valarray>

#include "snippet/aliases.hpp"

#include "internal/types.hpp"

#include "numeric/modular/modint.hpp"


namespace lib {


namespace internal {


template<class T>
struct factorial_base {
    using value_type = T;
    using size_type = internal::size_t;

    inline static constexpr value_type calc(const size_type& n) noexcept(NO_EXCEPT) {
        assert(n >= 0);
        value_type ans = 1;
        FOR(k, 1, n) ans *= k;
        return ans;
    }
};

} // namespace internal



template<class>
struct factorial {};


template<std::integral T>
struct factorial<T> : internal::factorial_base<T> {
    using value_type = T;
    using size_type = internal::size_t;

  private:
    size_type _n;
    std::valarray<value_type> _fact;

  public:
    factorial(const size_type n) noexcept(NO_EXCEPT) : _n(n), _fact(n + 1) {
        this->_fact[0] = this->_fact[1] = 1;
        FOR(i, 2, n) this->_fact[i] = this->_fact[i - 1] * i;
    }

    inline value_type fact(const size_type k) noexcept(NO_EXCEPT) {
        assert(0 <= k and k <= this->_n);
        return this->_fact[k];
    }
    inline value_type operator()(const size_type k) noexcept(NO_EXCEPT) {
        return this->fact(k);
    }

    auto _debug() const { return this->_fact; }
};


template<lib::internal::modint_family T>
struct factorial<T> : internal::factorial_base<T> {
    using value_type = T;
    using size_type = internal::size_t;

  private:
    size_type _n;
    std::valarray<value_type> _fact, _ifact, _inv;

  public:
    factorial(const size_type n) noexcept(NO_EXCEPT) : _n(n), _fact(n + 1), _ifact(n + 1), _inv(n + 1) {
        constexpr auto P = T::mod();

        this->_fact[0] = this->_fact[1] = 1;
        this->_ifact[0] = this->_ifact[1] = 1;
        this->_inv[1] = 1;

        FOR(i, 2, n) {
            this->_inv[i] = -this->_inv[P % i] * (P / i);

            this->_fact[i] = this->_fact[i - 1] * i;
            this->_ifact[i] = this->_ifact[i - 1] * this->_inv[i];
        }
    }

    inline value_type fact(const size_type k) noexcept(NO_EXCEPT) {
        assert(0 <= k and k <= this->_n);
        return this->_fact[k];
    }
    inline value_type operator()(const size_type k) noexcept(NO_EXCEPT) {
        return this->fact(k);
    }

    inline value_type ifact(const size_type k) noexcept(NO_EXCEPT) {
        assert(0 <= k and k <= this->_n);
        return this->_ifact[k];
    }

    inline value_type inv(const size_type k) noexcept(NO_EXCEPT) {
        assert(0 <= k and k <= this->_n);
        return this->_inv[k];
    }

    inline value_type bimom(const size_type n, const size_type r) noexcept(NO_EXCEPT) {
        assert(0 <= r and r <= n and n <= this->_n);
        return this->fact(n) * this->ifact(r) * this->ifact(n - r);
    }

    auto _debug() const { return this->_fact; }
};


} // namespace lib
