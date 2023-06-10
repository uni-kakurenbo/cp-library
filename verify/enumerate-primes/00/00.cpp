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
// #include <boost/range/adaptor/reversed.hpp>

void solve() {
    int n, a, b; cin >> n >> a >> b;
    assert(a > 0);
    auto sieve = lib::prime_enumerator(n);
    // debug(sieve.size());
    // debug(sieve);
    // debug(sieve | boost::adaptors::reversed);
    print(sieve.size(), (sieve.size() + a - b - 1) / a);
    for(auto itr = std::next(sieve.begin(), b); itr != sieve.end(); std::advance(itr, a)) {
        print << *itr << " ";
    }
}
