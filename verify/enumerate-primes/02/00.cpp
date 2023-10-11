/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */
/* #region template */
#include <bits/stdc++.h>

#include "template.hpp"
/* #endregion */

void solve();

signed main() {
    debug(__cplusplus);
    int $ = 1;
    // std::cin >> $;
    for(int _ = 0; _ < $; ++_) {
        DEBUG("Case: #" + std::to_string(_));
        solve();
    }
    return 0;
}

void solve() {
    i64 n; cin >> n;

    const i64 r = std::ceil(std::pow(n, 1.0 / 5));
    const i64 m = lib::sqrt_ceil((n + 11) / 12);

    lib::prime_enumerator<i64> p(m);

    i64 ans = 0;

    for(auto a=p.begin(), end=p.end(); a!=end; ++a) {
        if(*a > r) break;
        const i64 aa = *a * *a;
        for(auto c=std::next(a); c!=end; ++c) {
            const i64 aacc = aa * *c * *c;
            const i64 aaacc = *a * aacc;
            if(aaacc > n) break;
            if(n < (*c - 1) * aacc) {
                if(aaacc + aacc <= n) ans += p.count(n / aacc) - a.index() - 1;
            }
            else if(*a + 1 < *c - 1) ans += a - c - 1;
        }
    }

    print(ans);
}
