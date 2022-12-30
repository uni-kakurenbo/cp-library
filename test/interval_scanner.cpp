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

output_adapter _print;
#define print _print
/* #endregion */

// https://atcoder.jp/contests/abc032/tasks/abc032_c

#include "numeric/interval_scanner.hpp"

signed main() {
    int n; ll k; cin >> n >> k;

    vector<ll> a(n);
    REP(i, n) cin >> a[i];

    if (find(a.begin(), a.end(), 0ll) != a.end()) {
        cout << n << endl;
        return 0;
    }

    ll prod = 0;
    auto validate = [&](int val) { return val <= k; };
    auto init = [&]() { prod = 1; };
    auto valid = [&]() { return prod <= k; };
    auto expand = [&](int val) { prod *= val; };
    auto contract = [&](int val) { assert(prod % val == 0), prod /= val; };

    int ans = 0;
    auto apply = [&](auto left, auto right) { chmax(ans, right - left); };

    IntervalScanner<int> scanner(validate, init, valid, expand, contract, apply);
    scanner.scan(ALL(a));

    print(ans);
}
