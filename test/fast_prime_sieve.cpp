/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template/standard.hpp"
/* #endregion */

void solve();

signed main() {
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        _debug("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

#include "numeric/prime_enumerator.hpp"
#include <boost/range/adaptor/reversed.hpp>


void solve() {
    long n; cin >> n;
    debug(lib::prime_enumerator(n).size());
    // debug(lib::prime_enumerator(n));
    // debug(lib::prime_enumerator(n) | boost::adaptors::reversed);
}
