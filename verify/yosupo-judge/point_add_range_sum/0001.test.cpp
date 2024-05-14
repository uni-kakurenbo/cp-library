/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/point_add_range_sum"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/vector.hpp"
#include "adaptor/io.hpp"
#include "data_structure/segment_tree.hpp"
#include "action/range_sum.hpp"


signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::vector<uni::ll> a(n); input >> a;

    uni::segment_tree<uni::actions::range_sum<uni::ll>> data(a);

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::ll p, x; input >> p >> x;
            data[p] += x;
        }
        if(t == 1) {
            uni::i32 l, r; input >> l >> r;
            print(data.fold(l, r));
        }
    }

    return 0;
}
