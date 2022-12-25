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

#include <atcoder/modint>

#include "utility/applier.hpp"
#include "iterable/accumulation.hpp"
#include "data_structure/range_sum.hpp"

using mint = atcoder::modint998244353;

signed main() {
    int n; cin >> n;
    vector<int> a(n); REP(i, n) cin >> a[i];

    RangeSum<mint> cnt(200001);
    RangeSum<mint> sum(200001);

    mint cur = 0;
    FOR(k, 1, n) {
        mint smaller = cnt.sum(0, a[k-1]);
        mint larger_sum = sum.sum(a[k-1], 200001);

        cur += larger_sum * 2;
        cur += smaller * 2 * a[k-1];
        cur += a[k-1];

        print(cur / mint{k}.pow(2));

        cnt.add(a[k-1], mint{1});
        sum.add(a[k-1], mint{a[k-1]});
    }

    return 0;
}
