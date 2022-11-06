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

// https://atcoder.jp/contests/arc092/tasks/arc092_a

#include "graph/bipartite_matching.hpp"

signed main() {
    int n; cin >> n;
    vector<int> a(n), b(n), c(n), d(n);
    REP(i, n) cin >> a[i] >> b[i];
    REP(i, n) cin >> c[i] >> d[i];

    MaximumBipartiteMatching bm(n);

    REP(i, n) REP(j, n) {
        if(a[i] < c[j] and b[i] < d[j]) {
            bm.add(i, j);
        }
    }

    print(bm.solve());

    return 0;
}
