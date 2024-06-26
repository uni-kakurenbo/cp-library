#pragma once


#include <cassert>
#include <valarray>


#include <atcoder/math>


#include "snippet/aliases.hpp"
#include "snippet/iterations.hpp"

#include "numeric/modular/modint_interface.hpp"
#include "numeric/modular/barrett_reduction.hpp"
#include "numeric/modular/montgomery_reduction.hpp"
#include "numeric/modular/binary_reduction.hpp"


// Thanks to: https://nyaannyaan.github.io/library/modulo/arbitrary-mod-binomial.hpp
namespace uni {

namespace internal {


template<class T, class R = T, class Reduction = barrett_reduction_32bit>
    requires (std::numeric_limits<R>::digits > 30) || modint_family<R>
struct binomial_coefficient_prime_power_mod {
    using value_type = T;
    using mod_type = R;

    static constexpr i64 MOD_SUP = (i64{1} << 30) - 1;
    static constexpr u32 MAX_BUFFER_SIZE = 30'000'000;

  private:
    using self = binomial_coefficient_prime_power_mod;
    using reduction = Reduction;

    u32 _p, _q, _m;
    value_type _max;
    std::valarray<u32> _fact, _inv_fact, _inv;
    u32 _delta;
    barrett_reduction_32bit _barrett_m, _barrett_p;
    reduction _reduction_m;
    u32 _one;

    static constexpr std::pair<u32,u32> _factorize(u32 m) {
        for(u32 i=2; i*i<=m; ++i) {
            if(m % i == 0) {
                u32 cnt = 0;
                while(m % i == 0) m /= i, ++cnt;
                assert(m == 1);
                return { i, cnt };
            }
        }

        return { m, 1 };
    }

    void _init() {
        const u32 size = std::min(this->_m, static_cast<u32>(this->_max) + 1);
        assert(size < self::MAX_BUFFER_SIZE);

        this->_barrett_m = barrett_reduction_32bit(this->_m);
        this->_barrett_p = barrett_reduction_32bit(this->_p);
        this->_reduction_m = self::reduction(this->_m);

        this->_delta = this->_reduction_m.convert_raw((this->_p == 2 && this->_q >= 3) ? 1 : this->_m - 1);
        this->_one = this->_reduction_m.convert_raw(1);

        this->_fact.resize(size);
        this->_inv_fact.resize(size);
        this->_inv.resize(size);

        this->_fact[0] = this->_inv_fact[0] = this->_inv[0] = this->_one;
        this->_fact[1] = this->_inv_fact[1] = this->_inv[1] = this->_one;

        REP(i, 2, size) {
            if(this->_barrett_p.reduce(i) == 0) {
                this->_fact[i] = this->_fact[i - 1];
                this->_fact[i + 1] = this->_reduction_m.multiply(this->_fact[i - 1], this->_reduction_m.convert_raw(this->_barrett_m.reduce(i + 1)));
                ++i;
            }
            else {
                this->_fact[i] = this->_reduction_m.multiply(this->_fact[i - 1], this->_reduction_m.convert_raw(this->_barrett_m.reduce(i)));
            }
        }

        this->_inv_fact[size - 1] = this->_reduction_m.pow(this->_fact[size - 1], this->_m / this->_p * (this->_p - 1) - 1);

        REPD(i, 2, size - 1) {
            this->_inv_fact[i] = this->_reduction_m.multiply(this->_inv_fact[i + 1], this->_reduction_m.convert_raw(this->_barrett_m.reduce(i + 1)));
            if(this->_barrett_p.reduce(i) == 0) {
                this->_inv_fact[i - 1] = this->_inv_fact[i];
                --i;
            }
        }
    }

  public:
    binomial_coefficient_prime_power_mod() noexcept = default;

    explicit binomial_coefficient_prime_power_mod(const value_type max = 20'000'000) noexcept(NO_EXCEPT)
        requires (1 < mod_type::mod() && mod_type::mod() < self::MOD_SUP)
      : _m(mod_type::mod()), _max(max)
    {
        constexpr std::pair<u32,u32> pq = self::_factorize(mod_type::mod());
        std::tie(this->_p, this->_q) = pq;

        this->_init();
    }

    explicit binomial_coefficient_prime_power_mod(const mod_type p, mod_type q = 1, const value_type max = 20'000'000) noexcept(NO_EXCEPT)
        requires (not modint_family<mod_type>)
      : _p(static_cast<u32>(p)), _q(static_cast<u32>(q)), _max(max)
    {
        assert(1 < p && p < self::MOD_SUP);
        assert(0 < q);

        u64 m = 1;
        while(q--) {
            m *= this->_p;
            assert(m < MOD_SUP);
        }
        this->_m = static_cast<u32>(m);

        this->_init();
    }

    inline mod_type mod() const noexcept(NO_EXCEPT) { return this->_m; }

    mod_type lucus(value_type n, value_type r) const noexcept(NO_EXCEPT) {
        assert(0 <= n);
        assert(0 <= r);

        if(n < r) return 0;
        u32 res = this->_one;

        while(n > 0) {
            u32 n0, k0;
            std::tie(n, n0) = this->_barrett_p.divide(n);
            std::tie(r, k0) = this->_barrett_p.divide(r);
            if(n0 < k0) return 0;

            res = this->_reduction_m.multiply(res, this->_fact[n0]);
            res = this->_reduction_m.multiply(res, this->_reduction_m.multiply(this->_inv_fact[n0 - k0], this->_inv_fact[k0]));
        }

        return static_cast<mod_type>(this->_reduction_m.revert(res));
    }

    mod_type comb(value_type n, value_type r) const noexcept(NO_EXCEPT) {
        assert(0 <= n);
        assert(0 <= r);

        if(this->_q == 1) return this->lucus(n, r);
        if(n < r) return 0;

        value_type k = n - r;
        u32 e0 = 0, eq = 0, i = 0;
        u32 res = this->_one;

        while(n > 0) {
            res = this->_reduction_m.multiply(res, this->_fact[this->_barrett_m.reduce(n)]);
            res = this->_reduction_m.multiply(res, this->_inv_fact[this->_barrett_m.reduce(r)]);
            res = this->_reduction_m.multiply(res, this->_inv_fact[this->_barrett_m.reduce(k)]);

            n = this->_barrett_p.quotient(n);
            r = this->_barrett_p.quotient(r);
            k = this->_barrett_p.quotient(k);

            u32 eps = static_cast<u32>(n - r - k);
            e0 += eps;
            if(e0 >= this->_q) return 0;
            if(++i >= this->_q) eq += eps;
        }

        if(eq & 1) res = this->_reduction_m.multiply(res, this->_delta);
        res = this->_reduction_m.multiply(res, this->_reduction_m.pow(this->_reduction_m.convert_raw(this->_p), e0));

        return static_cast<mod_type>(this->_reduction_m.revert(res));
    }
};


} // namespace internal


using internal::binomial_coefficient_prime_power_mod;


// (md < 10^7 && N < 2^30) || (md < 2^30 && N < 2 * 10^7)
template<class T, class R = T>
    requires (std::numeric_limits<R>::digits > 30) || internal::modint_family<R>
struct binomial_coefficient {
    using value_type = T;
    using mod_type = R;

  private:
    template<class reduction = montgomery_reduction_32bit>
    using internal_binomial = internal::binomial_coefficient_prime_power_mod<value_type, long long, reduction>;

    u32 _mod;
    value_type _max;
    std::vector<u32> _mods;
    std::vector<internal_binomial<>> _internals;
    internal_binomial<binary_reduction_32bit> _internal_2p;

    void _init() noexcept(NO_EXCEPT) {
        u32 md = this->_mod;
        if(md == 1) return;

        assert((md < 20'000'000 && this->_max < (1 << 30)) || (md < (1 << 30) && this->_max < 30'000'000));

        assert(1 <= md);
        assert(md <= internal_binomial<>::MOD_SUP);

        for(u32 i=2; i*i<=md; ++i) {
            if(md % i == 0) {
                u32 j = 0, k = 1;
                while(md % i == 0) md /= i, ++j, k *= i;
                this->_mods.push_back(k);
                if(i == 2) {
                    this->_internal_2p = internal_binomial<binary_reduction_32bit>(2, j, this->_max);
                    this->_internals.emplace_back();
                }
                else {
                    this->_internals.emplace_back(i, j, this->_max);
                    assert(this->_mods.back() == this->_internals.back().mod());
                }
            }
        }
        if(md != 1) {
            this->_mods.push_back(static_cast<u32>(md));
            if(md == 2) this->_internal_2p = internal_binomial<binary_reduction_32bit>(2, 1, this->_max);
            else this->_internals.emplace_back(md, 1, this->_max);
        }
    }

  public:
    binomial_coefficient(const value_type max = 20'000'000) noexcept(NO_EXCEPT)
        requires internal::modint_family<mod_type>
      : _mod(static_cast<u32>(mod_type::mod())), _max(max)
    {
        this->_init();
    }

    binomial_coefficient(const mod_type mod, const value_type max = 20'000'000) noexcept(NO_EXCEPT)
        requires (not internal::modint_family<mod_type>)
      : _mod(static_cast<u32>(mod)), _max(max)
    {
        this->_init();
    }

    mod_type comb(const value_type n, const value_type r) const noexcept(NO_EXCEPT) {
        assert(n >= 0), assert(r >= 0);
        if(this->_mod == 1) return 0;
        if(n < r) return 0;

        if(this->_mods.size() == 1) {
            if((this->_mods.back() & 1) == 0) return static_cast<mod_type>(this->_internal_2p.comb(n, r));
            else return static_cast<mod_type>(this->_internals[0].comb(n, r));
        }

        std::vector<long long> rem, div;
        REP(i, 0, std::ranges::ssize(this->_mods)) {
            div.push_back(this->_mods[i]);
            if((this->_mods[i] & 1) == 0) rem.push_back(this->_internal_2p.comb(n, r));
            else {
                rem.push_back(this->_internals[i].comb(n, r));
            }
        }

        return static_cast<mod_type>(atcoder::crt(rem, div).first);
    }
};


} // namespace uni
