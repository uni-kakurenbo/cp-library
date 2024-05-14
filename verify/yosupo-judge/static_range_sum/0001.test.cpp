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
#include "data_structure/disjoint_sparse_table.hpp"
#include "algebraic/addition.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    std::vector<uni::i32> a(n); REP(i, n) input >> a[i];
    uni::disjoint_sparse_table<uni::algebraic::addition<uni::i64>> sum(a);

    REP(q) {
        uni::i32 l, r; input >> l >> r;
        print(sum(l, r).fold());
    }
}
