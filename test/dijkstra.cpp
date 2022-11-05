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

// https://atcoder.jp/contests/abc237/tasks/abc237_e

#include "utility/applier.hpp"

#include "graph.hpp"
#include "graph/dijkstra.hpp"

signed main() {
    int n, m; cin >> n >> m;
    vector<ll> h(n); ITRR(v, h) cin >> v;

    Graph<> G(n);
    LOOP(m) {
        int u, v; cin >> u >> v; --u, --v;
        if(h[v] > h[u]) swap(u, v);
        G.add_edge(v, u, h[u]-h[v]);
        G.add_edge(u, v, 0);
    }
    debug(G);

    vector<ll> res(n, INF64);
    G.dijkstra(0, &res);

    debug(res);

    Max<ll> ans = 0;
    REP(i, n) ans <<= h[0] - h[i] - res[i];

    print(ans);

    return 0;
}
