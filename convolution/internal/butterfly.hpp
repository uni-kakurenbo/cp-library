#pragma once


#include <algorithm>
#include <array>
#include <cassert>
#include <array>
#include <type_traits>
#include <vector>
#include <bit>
#include <concepts>
#include <ranges>


#include "snippet/aliases.hpp"

#include "numeric/internal/primitive_root.hpp"
#include "numeric/modular/modint.hpp"

#include "utility/functional.hpp"

#include "adaptor/valarray.hpp"


namespace uni {

namespace internal {


// Thanks to: atocder/convolution
template<uni::internal::modint_family mint>
struct fft_info {
    static constexpr mint g = primitive_root<mint>(mint::mod());
    static constexpr auto rank2 = std::countr_zero(mint::mod() - 1);

    std::array<mint, rank2 + 1> root;
    std::array<mint, rank2 + 1> iroot;

    std::array<mint, std::max(0, rank2 - 2 + 1)> rate2;
    std::array<mint, std::max(0, rank2 - 2 + 1)> irate2;

    std::array<mint, std::max(0, rank2 - 3 + 1)> rate3;
    std::array<mint, std::max(0, rank2 - 3 + 1)> irate3;

    consteval fft_info() {
        this->root[rank2] = g.pow((mint::mod() - 1) >> this->rank2);
        this->iroot[rank2] = this->root[this->rank2].inv();

        REPD(i, rank2) {
            this->root[i] = this->root[i + 1] * this->root[i + 1];
            this->iroot[i] = this->iroot[i + 1] * this->iroot[i + 1];
        }

        {
            mint prod = mint::one, iprod = mint::one;
            REP(i, rank2 - 1) {
                this->rate2[i] = this->root[i + 2] * prod;
                this->irate2[i] = this->iroot[i + 2] * iprod;
                prod *= this->iroot[i + 2];
                iprod *= this->root[i + 2];
            }
        }
        {
            mint prod = mint::one, iprod = mint::one;
            REP(i, rank2 - 2) {
                this->rate3[i] = this->root[i + 3] * prod;
                this->irate3[i] = this->iroot[i + 3] * iprod;
                prod *= this->iroot[i + 3];
                iprod *= this->root[i + 3];
            }
        }
    }
};


}  // namespace internal


template<std::ranges::sized_range R>
    requires internal::static_modint_family<std::ranges::range_value_t<R>>
void butterfly(R& v1) {
    using mint = std::ranges::range_value_t<R>;

    const auto n = std::ranges::ssize(v1);
    const auto h = to_signed(std::countr_zero(to_unsigned(n)));

    static constexpr internal::fft_info<mint> info;

    internal::size_t len = 0;
    while(len < h) {
        if(h - len == 1) {
            const internal::size_t p = 1 << (h - len - 1);
            mint rot = mint::one;

            REP(s, 1 << len) {
                const internal::size_t offset = s << (h - len);
                REP(i, p) {
                    const auto l = v1[i + offset];
                    const auto r = v1[i + offset + p] * rot;
                    v1[i + offset] = l + r;
                    v1[i + offset + p] = l - r;
                }
                if(s + 1 != (1 << len)) {
                    rot *= info.rate2[std::countr_zero(~to_unsigned(s))];
                }
            }

            ++len;
        }
        else {
            const internal::size_t p = 1 << (h - len - 2);
            const mint imag = info.root[2];
            mint rot = mint::one;

            REP(s, 1 << len) {
                const mint rot2 = rot * rot;
                const mint rot3 = rot2 * rot;
                const internal::size_t offset = s << (h - len);

                REP(i, p) {
                    const auto mod2 = 1ULL * mint::mod() * mint::mod();
                    const auto a0 = 1ULL * v1[i + offset].val();
                    const auto a1 = 1ULL * v1[i + offset + p].val() * rot.val();
                    const auto a2 = 1ULL * v1[i + offset + 2 * p].val() * rot2.val();
                    const auto a3 = 1ULL * v1[i + offset + 3 * p].val() * rot3.val();

                    const auto a1na3imag = 1ULL * mint{ a1 + mod2 - a3 }.val() * imag.val();

                    const auto na2 = mod2 - a2;

                    v1[i + offset] = a0 + a2 + a1 + a3;
                    v1[i + offset + 1 * p] = a0 + a2 + (2 * mod2 - (a1 + a3));
                    v1[i + offset + 2 * p] = a0 + na2 + a1na3imag;
                    v1[i + offset + 3 * p] = a0 + na2 + (mod2 - a1na3imag);
                }

                if(s + 1 != (1 << len)) rot *= info.rate3[std::countr_zero(~to_unsigned(s))];
            }

            len += 2;
        }
    }
}


template<std::ranges::sized_range R>
    requires internal::static_modint_family<std::ranges::range_value_t<R>>
void butterfly_inv(R& v1) {
    using mint = std::ranges::range_value_t<R>;

    const auto n = std::ranges::ssize(v1);
    const auto h = std::countr_zero(to_unsigned(n));

    static constinit internal::fft_info<mint> info;

    internal::size_t len = h;
    while(len > 0) {
        if(len == 1) {
            const internal::size_t p = 1 << (h - len);
            mint irot = mint::one;

            REP(s, 1 << (len - 1)) {
                const internal::size_t offset = s << (h - len + 1);
                REP(i, p) {
                    const auto l = v1[i + offset];
                    const auto r = v1[i + offset + p];
                    v1[i + offset] = l + r;
                    v1[i + offset + p] = to_unsigned(mint::mod() + l.val() - r.val()) * irot.val();
                }
                if(s + 1 != (1 << (len - 1))) irot *= info.irate2[std::countr_zero(~to_unsigned(s))];
            }

            --len;
        }
        else {
            const auto p = 1 << (h - len);
            const mint iimag = info.iroot[2];
            mint irot = mint::one;

            REP(s, 1 << (len - 2)) {
                const mint irot2 = irot * irot;
                const mint irot3 = irot2 * irot;
                const internal::size_t offset = s << (h - len + 2);
                REP(i, p) {
                    const auto a0 = 1ULL * v1[i + offset + 0 * p].val();
                    const auto a1 = 1ULL * v1[i + offset + 1 * p].val();
                    const auto a2 = 1ULL * v1[i + offset + 2 * p].val();
                    const auto a3 = 1ULL * v1[i + offset + 3 * p].val();

                    const auto a2na3iimag = 1ULL * mint{ (mint::mod() + a2 - a3) * iimag.val() }.val();

                    v1[i + offset] = a0 + a1 + a2 + a3;
                    v1[i + offset + 1 * p] = (a0 + (mint::mod() - a1) + a2na3iimag) * irot.val();
                    v1[i + offset + 2 * p] = (a0 + a1 + (mint::mod() - a2) + (mint::mod() - a3)) * irot2.val();
                    v1[i + offset + 3 * p] = (a0 + (mint::mod() - a1) + (mint::mod() - a2na3iimag)) * irot3.val();
                }

                if(s + 1 != (1 << (len - 2))) irot *= info.irate3[std::countr_zero(~to_unsigned(s))];
            }

            len -= 2;
        }
    }
}


template<class Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires
        std::same_as<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        internal::static_modint_family<std::ranges::range_value_t<R0>>
Res convolution_naive(R0&& v0, R1&& v1) {
    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    Res ans(n + m - 1);

    if(n < m) {
        REP(j, m) REP(i, n) ans[i + j] += v0[i] * v1[j];
    }
    else {
        REP(i, n) REP(j, m) ans[i + j] += v0[i] * v1[j];
    }

    return ans;
}


template<class Res, std::ranges::sized_range R0, std::ranges::sized_range R1>
    requires
        std::same_as<std::ranges::range_value_t<R0>, std::ranges::range_value_t<R1>> &&
        internal::static_modint_family<std::ranges::range_value_t<R0>> &&
        internal::resizable_range<R0> && internal::resizable_range<R1> &&
        std::convertible_to<std::ranges::range_value_t<R0>, std::ranges::range_value_t<Res>>
Res convolution_fft(R0 v0, R1 v1) {
    using mint = std::ranges::range_value_t<R0>;

    const auto n = std::ranges::ssize(v0);
    const auto m = std::ranges::ssize(v1);

    const auto z = to_signed(std::bit_ceil(to_unsigned(n + m - 1)));

    v0.resize(z);
    butterfly(v0);

    v1.resize(z);
    butterfly(v1);

    REP(i, z) v0[i] *= v1[i];

    butterfly_inv(v0);
    v0.resize(n + m - 1);

    const mint iz = mint{ z }.inv();
    REP(i, n + m - 1) v0[i] *= iz;

    if constexpr(std::convertible_to<R0, Res>) return v0;
    else return Res(ALL(v0));
}


}  // namespace uni
