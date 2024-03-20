/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/static_range_sum"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/valarray.hpp"
#include "iterable/accumulation.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<int> a(n); input >> a;
    lib::accumulation<lib::ll> sum(ALL(a));

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r));
    }
}
