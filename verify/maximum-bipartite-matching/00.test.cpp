/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#include "sneaky/enforce_int128_enable.hpp"

#define PROBLEM "https://judge.yosupo.jp/problem/bipartitematching"

#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "graph/maximum_bipartite_matching.hpp"

signed main() {
    int l, r, n; input >> l >> r >> n;

    lib::maximum_bipartite_matching matching(l, r);
    REP(n) {
        int a, b; input >> a >> b;
        matching.add(a, b);
    }

    auto res = matching.get_matching();

    print(res.size());
    ITR(u, v, res) print(u, v);
}
