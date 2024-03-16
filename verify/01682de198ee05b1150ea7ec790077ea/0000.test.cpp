/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=GRL_1_A"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "adapter/vector.hpp"
#include "snippet/aliases.hpp"
#include "structure/graph.hpp"
#include "graph/shortest_path.hpp"
#include "iterable/operation.hpp"

signed main() {
    int n, m, s; input >> n >> m >> s;
    lib::graph<lib::i64> graph(n); graph.read<true, false>(m);

    auto dists = graph.shortest_path_with_cost(s);

    print(lib::join(dists | std::views::transform([](auto x) { return x >= lib::INF64 ? "INF" : std::to_string(x); }), "\n"));
}
