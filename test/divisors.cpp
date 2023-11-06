/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
/* #region template */
#include <bits/stdc++.h>
using namespace std;

#include "template/standard.hpp"

output_adapter _print;
#define print _print
/* #endregion */

// https://atcoder.jp/contests/abc106/tasks/abc106_b

#include "numeric/divisors.hpp"

signed main() {
    int n; cin >> n;
    int ans = 0;
    FOR(i, 1, n) {
        if(i%2 == 0) continue;
        divisors divs(i);
        ans += divs.size() == 8;
    }
    printf("%i\n", ans);
    return 0;
}
