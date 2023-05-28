/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */
#include <bits/stdc++.h>

#include "template.hpp"
#include "adapter/output.hpp"

#include "graph/topological_sort.hpp"

using namespace std;
output_adapter print;

// https://atcoder.jp/contests/abc223/tasks/abc223_d

signed main() {
    int n, m; cin >> n >> m;

    graph<> G(n);
    vector<int> in_degs(n);

    REP(i, m) {
        int a, b; cin >> a >> b; --a, --b;
        G.add_edge(a, b);
        ++in_degs[b];
    }

    vector<int> sorted;
    bool ok = G.sort_topologically_with_priority<greater<ll>>(&sorted);

    if(ok) {
        ITRR(v, sorted) ++v;
        print(ALL(sorted));
    }
    else print(-1);

    return 0;
}
