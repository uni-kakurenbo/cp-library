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
/* #endregion */

//https://atcoder.jp/contests/abc281/tasks/abc281_e

#include "data_structure/priority_sum.hpp"

signed main() {
    int n, m, k; cin >> n >> m >> k;
    vector<int> a(n); REP(i, n) cin >> a[i];

    lib::PrioritySum<ll> data;

    REP(i, m) data.add(a[i]);

    REP(i, n-m+1) {
        print(data.sum(k));
        data.erase_value(a[i]);
        data.add(a[i+m]);
    }

    return 0;
}
