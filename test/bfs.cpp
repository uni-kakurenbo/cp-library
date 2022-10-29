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

// https://atcoder.jp/contests/abc151/tasks/abc151_d

#include "utility/apply.hpp"

#include "grid.hpp"
#include "graph.hpp"
#include "graph/bfs.hpp"

signed main() {
    int h, w; cin >> h >> w;

    Grid<char,string> grid(h, w); grid.read();
    Graph<> graph;
    grid.build_graph_as_maze(&graph);

    Max<int> ans = -1;

    REP(si, h) REP(sj, w) {
        if(grid[si][sj] == '#') continue;
        vector<ll> dists;
        graph.bfs(grid.id(si,sj), &dists);
        REP(gi, h) REP(gj, w) {
            if(grid[gi][gj] == '#') continue;
            ans <<= dists[grid.id(gi,gj)];
        }
    }

    print(ans);

    return 0;
}
