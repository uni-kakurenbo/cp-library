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

#define PROBLEM "https://atcoder.jp/contests/abc239/tasks/abc239_e"

#include "data_structure/wavelet_matrix.hpp"

void solve() {

    int n, q; cin >> n >> q;
    std::valarray<int> x(n); input >> x;

    lib::graph<int> G(n); G.read_bidirectionally(n - 1);

    vector<int> vs;
    valarray<int> in(n), out(n);
    auto dfs = [&](auto&& dfs, int v, int p) -> void {
        in[v] = vs.size();
        vs.push_back(v);
        ITR(e, G[v]) if(e.to != p) dfs(dfs, e.to, v);
        vs.push_back(v);
        out[v] = vs.size();
    };
    dfs(dfs, 0, -1);

    vector<int> y(2 * n);
    REP(i, 2 * n) y[i] = x[vs[i]];

    lib::wavelet_matrix<u32>::compressed data(y);
    debug(data);

    REP(q) {
        int v, k; std::cin >> v >> k; --v, --k;
        print(data(in[v], out[v]).kth_largest(2 * k));
    }
}
