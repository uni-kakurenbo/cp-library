/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/staticrmq"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/valarray.hpp"
#include "data_structure/segment_tree.hpp"
#include "action/range_min.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::valarray<uni::i32> a(n); input >> a;

    uni::segment_tree<uni::actions::range_min<uni::i32>> min(a);

    REP(q) {
        uni::i32 l, r; input >> l >> r;
        print(min(l, r).fold());
    }
}
