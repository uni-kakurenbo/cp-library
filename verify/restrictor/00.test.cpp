/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://atcoder.jp/contests/abc293/tasks/abc293_f"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "numeric/arithmetic.hpp"
#include "numeric/bit.hpp"
#include "utility/restrictor.hpp"
#include "numeric/boundary_seeker.hpp"

void solve();

signed main() {
    int t; std::cin >> t;
    while(t--) solve();
}

void solve() {
    lib::i64 n; std::cin >> n;

    auto ok = [&](lib::i64 b, lib::i64 d) {
        auto s = lib::to_base_n_vector(n, b);
        if((long)s.size() != d) return false;
        return std::count(ALL(s), 0) + std::count(ALL(s), 1) == (long)s.size();
    };

    lib::i64 ans = 1;
    ans += (n >= 3);

    int d = lib::bit_width(lib::to_unsigned(n));

    constexpr lib::i64 SUP = 2'000'000'000'000'000'000L;

    FOR(i, 3, d) {
        lib::boundary_seeker<lib::i64> seeker(
            [&](auto x) { return lib::pow(lib::static_restrictor<lib::i64, 0, SUP>{x}, i-1) <= n; }
        );
        lib::i64 b = seeker.bound(2);
        ans += ok(b, i);
    }

    print(ans);
}
