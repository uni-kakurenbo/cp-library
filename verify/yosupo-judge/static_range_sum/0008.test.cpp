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
    int n, q; std::cin >> n >> q;
    lib::valarray<lib::i64> a(n); input >> a;
    lib::dynamic_segment_tree<lib::actions::range_sum<lib::i64>> sum(a);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r).fold());
    }
}
