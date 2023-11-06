/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://atcoder.jp/contests/abc185/tasks/abc185_f?lang=en&editorialLang=ja"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/vector.hpp"
#include "adapter/io.hpp"
#include "data_structure/segment_tree.hpp"
#include "data_structure/range_action/range_bitxor.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::vector<int> a(n); input >> a;

    lib::segment_tree<lib::actions::range_bitxor<int>> data(ALL(a));

    REP(q) {
        int t; std::cin >> t;
        if(t == 1) {
            int p, x; std::cin >> p >> x; --p;
            data[p] += x;
        }
        if(t == 2) {
            int l, r; std::cin >> l >> r; --l;
            print(data(l, r).fold());
        }
    }

    return 0;
}
