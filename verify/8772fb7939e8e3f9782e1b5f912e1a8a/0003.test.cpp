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
#include "adapter/io.hpp"
#include "adapter/valarray.hpp"
#include "iterable/accumulation.hpp"
#include "data_structure/segment_tree.hpp"
#include "algebraic/addition.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::valarray<lib::i64> a(n); input >> a;
    lib::segment_tree<lib::algebraic::addition<lib::i64>> sum(a);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r).fold());
    }
}
