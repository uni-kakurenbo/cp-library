#pragma once


#include <functional>
#include <type_traits>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "internal/dev_env.hpp"
#include "internal/types.hpp"

#include "adaptor/gnu/hash_table.hpp"

#include "numeric/limits.hpp"


namespace uni {


template<class Signature>
struct extremum_seeker;


template<class T, std::floating_point Arg>
struct extremum_seeker<T(Arg)> {
    using value_type = T;
    using arg_type = Arg;

  private:
    std::function<T(Arg)> _f;


    template<const internal::size_t ITERATIONS>
    std::pair<arg_type, arg_type> search(arg_type low, arg_type high, const auto comp) const noexcept(NO_EXCEPT) {
        REP(ITERATIONS) {
            const auto p = low + (high - low) / 3;
            const auto q = high - (high - low) / 3;

            if(comp(p, q)) high = q;
            else low = p;
        }

        return { low, high };
    }

  public:
    template<class F>
        requires std::same_as<std::invoke_result_t<F, Arg>, T>
    extremum_seeker(F&& f) noexcept(NO_EXCEPT) : _f(std::forward<F>(f)) {};


    template<const internal::size_t ITERATIONS = 100'000>
    std::pair<arg_type, arg_type> arg_min(
        arg_type low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        arg_type high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        const auto comp = [&](const arg_type lhs, const arg_type rhs) noexcept(NO_EXCEPT) {
            return this->_f(lhs) < this->_f(rhs);
        };
        return this->search<ITERATIONS>(low, high, comp);
    }

    template<const internal::size_t ITERATIONS = 100'000>
    std::pair<arg_type, arg_type> arg_max(
        arg_type low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        arg_type high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        const auto comp = [&](const arg_type lhs, const arg_type rhs) noexcept(NO_EXCEPT) {
            return this->_f(lhs) > this->_f(rhs);
        };
        return this->search<ITERATIONS>(low, high, comp);
    }


    template<const internal::size_t ITERATIONS = 100'000>
    auto min(
        const arg_type _low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        const arg_type _high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        const auto [ low, high ] = this->arg_min<ITERATIONS>(_low, _high);
        auto res = std::min(this->_f(low), this->_f(high));
        FOR(x, low, high) chmin(res, this->_f(x));
        return res;
    }

    template<const internal::size_t ITERATIONS = 100'000>
    auto max(
        const arg_type _low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        const arg_type _high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) const noexcept(NO_EXCEPT) {
        const auto [ low, high ] = this->arg_max<ITERATIONS>(_low, _high);
        auto res = std::max(this->_f(low), this->_f(high));
        FOR(x, low, high) chmax(res, this->_f(x));
        return res;
    }
};


template<class T, std::integral Arg>
struct extremum_seeker<T(Arg)> {
    using value_type = T;
    using arg_type = Arg;

  private:
    std::function<T(Arg)> _f;

    std::vector<arg_type> _fib = { 0, 1 };
    gnu::gp_hash_table<arg_type, value_type> _cache;


    auto _expand(const arg_type size) noexcept(NO_EXCEPT) {
        assert(size >= 0);
        this->_fib.reserve(std::bit_width(to_unsigned(size)));

        if(this->_fib.back() < size) {
            do {
                this->_fib.emplace_back(
                    this->_fib[std::ranges::size(this->_fib) - 1] +
                    this->_fib[std::ranges::size(this->_fib) - 2]
                );
            } while(this->_fib.back() < size);

            return std::ranges::prev(this->_fib.end());
        }

        return std::ranges::lower_bound(this->_fib, size);
    }

    auto _func(const arg_type size) noexcept(NO_EXCEPT) {
        if(this->_cache.contains(size)) return this->_cache[size];
        return this->_cache[size] = this->_f(size);
    }

    auto _search(arg_type low, const arg_type high, const auto comp) noexcept(NO_EXCEPT) {
        if(low == high) return low;

        auto _low = low - 1;
        const auto itr = this->_expand(high - _low + 1);
        auto _high = *itr + _low;
        auto diff = *std::ranges::prev(itr);

        while(true) {
            const auto p = _high - diff;
            const auto q = _low + diff;

            if(p == q) return p;

            if(comp(p, q)) _high = q;
            else _low = p;

            diff = diff - (q - p);
        }
    }

  public:
    template<class F>
        requires std::same_as<std::invoke_result_t<F, Arg>, T>
    extremum_seeker(F&& f, const arg_type init_size = 0) noexcept(NO_EXCEPT)
      : _f(std::forward<F>(f))
    {
        this->_expand(init_size);
    }


    auto arg_min(
        const arg_type low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        const arg_type high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) noexcept(NO_EXCEPT) {
        const auto comp = [&](const arg_type lhs, const arg_type rhs) noexcept(NO_EXCEPT) {
            if(high < rhs) return true;
            return this->_func(lhs) < this->_func(rhs);
        };
        return this->_search(low, high, comp);
    }

    auto arg_max(
        const arg_type low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        const arg_type high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) noexcept(NO_EXCEPT) {
        const auto comp = [&](const arg_type lhs, const arg_type rhs) noexcept(NO_EXCEPT) {
            if(high < rhs) return true;
            return this->_func(lhs) > this->_func(rhs);
        };
        return this->_search(low, high, comp);
    }


    inline auto min(
        const arg_type _low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        const arg_type high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) noexcept(NO_EXCEPT) {
        return this->_func(this->arg_min(_low, high));
    }

    inline auto max(
        const arg_type _low = uni::numeric_limits<arg_type>::arithmetic_negative_infinity(),
        const arg_type high = uni::numeric_limits<arg_type>::arithmetic_infinity()
    ) noexcept(NO_EXCEPT) {
        return this->_func(this->arg_max(_low, high));
    }
};


} // namespace uni
