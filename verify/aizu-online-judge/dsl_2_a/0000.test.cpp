/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_A"

#include <iostream>
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/segment_tree.hpp"
#include "action/range_min.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    uni::segment_tree<uni::actions::range_min<int>> data(n);
    // debug(data);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, x; std::cin >> p >> x;;
            data[p] = x;
        }
        if(t == 1) {
            int l, r; std::cin >> l >> r; ++r;
            print(data(l, r).fold());
        }
        // debug(data);
    }

    return 0;
}
