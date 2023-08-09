/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
// #include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "numeric/prime_enumerator.hpp"
#include <boost/range/adaptor/reversed.hpp>
#include "random/xorshift.hpp"
#include "random/generator.hpp"

void solve() {
    int n; cin >> n;
    lib::random_engine<lib::xorshift> rng(n);
    REP(i, n) {
        const int n = rng(1'000'000) + 1;
        debug(i, n);

        const auto p = lib::prime_sieve(n);
        const auto q = lib::prime_enumerator(n);

        // debug(q);
        // debug(q | boost::adaptors::reversed);

        assert(std::equal(ALL(p), ALL(q)));
        assert(std::equal(ALL(p | boost::adaptors::reversed), ALL(q | boost::adaptors::reversed)));
    }
}
