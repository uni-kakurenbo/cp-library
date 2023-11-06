/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://atcoder.jp/contests/abc223/tasks/abc223_f"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/string.hpp"
#include "iterable/accumulation.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "data_structure/range_action/range_add_range_min.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::valarray<lib::i64> d(n);
    REP(i, n) {
        char c; std::cin >> c;
        d[i] = (c == '(') ? 1 : -1;
    }
    lib::accumulation cum(d);

    lib::lazy_segment_tree<lib::actions::range_add_range_min<int>> v(cum);

    REP(q) {
        int t; std::cin >> t;
        int l, r; std::cin >> l >> r; --l;
        if(t == 1) {
            v(l+1, r) += (v[r].val().val() - v[r-1].val().val()) - (v[l+1].val().val() - v[l].val().val());
        }
        if(t == 2) {
            print((v(l, r).fold().val() >= v[l].val().val() and v[r].val() == v[l].val()) ? "Yes" : "No");
        }
    }
}
