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
#include "adapter/

output_adapter _print;
#define print _print
/* #endregion */

// https://codingcompetitions.withgoogle.com/codejam/round/0000000000432cc7/0000000000432a83#problem

#include "structure/graph.hpp"
#include "graph/minimum_paph_cover.hpp"
using namespace lib;

struct Solver {
    int n, k;
    vector<valarray<int>> data;

    Solver(const int n, const int k) : n(n), k(k), data(n, valarray<int>(0, k)) {}

    void read() {
        REP(i, n) REP(j, k) cin >> data[i][j];
    }

    int solve() {
        graph G(n);
        REP(i, n) REP(j, n) {
            valarray<bool> less_i = data[i] < data[j];
            if(less_i.min()) G.add_edge(i, j);
        }
        return G.minimum_paph_cover_size_as_dag();
    }
};

signed main() {
    int t; cin >> t;
    FOR(i, 1, t) {
        int n, k; cin >> n >> k;
        Solver solver(n, k);
        solver.read();
        print("Case #"+to_string(i)+":", solver.solve());
    }
    return 0;
}
