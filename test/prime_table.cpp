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

#include "template.hpp"
#include "output.hpp"

Output _print;
#define print _print
/* #endregion */

// https://atcoder.jp/contests/abc239/tasks/abc239_d

#include "numeric/prime_table.hpp"

signed main() {
    int a, b, c, d; cin >> a >> b >> c >> d;
    Primes primes(b+d);
    int max_np = 0, np = 0;
    FOR(i, a+c, b+d) {
        if(primes(i)) np = 0;
        else np++;
        chmax(max_np, np);
    }
    print((max_np <= d-c ? "Aoki" : "Takahashi"));
    return 0;
}
