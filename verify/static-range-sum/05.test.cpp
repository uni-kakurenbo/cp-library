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
#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/helpers.hpp"
#include "data_structure/range_action/range_sum.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::valarray<lib::i64> a(n); input >> a;
    lib::lazy_segment_tree<lib::actions::make_full_t<lib::actions::range_sum<lib::i64>>> sum(a);

    REP(q) {
        int l, r; std::cin >> l >> r;
        print(sum(l, r).fold());
    }
}
