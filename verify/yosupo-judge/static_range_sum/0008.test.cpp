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
#include "data_structure/dynamic_segment_tree.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/helpers.hpp"
#include "action/range_sum.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::valarray<uni::i64> a(n); input >> a;
    uni::dynamic_segment_tree<uni::actions::range_sum<uni::i64>> sum(a);

    REP(q) {
        uni::i32 l, r; input >> l >> r;
        print(sum(l, r).fold());
    }
}
