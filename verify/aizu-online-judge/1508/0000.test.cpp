/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1508"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "data_structure/actable_dynamic_sequence.hpp"
#include "action/range_min.hpp"
#include "action/helpers.hpp"

signed main() {
    int n, q; input >> n >> q;
    lib::vector<int> a(n); input >> a;

    lib::actable_dynamic_sequence<lib::actions::make_full_t<lib::actions::range_min<int>>> data(a);
    debug(data.dump_rich());

    REP(q) {
        int t; input >> t;
        if(t == 0) {
            int l, r; input >> l >> r; ++r;
            data.rotate_right(l, r);
        }
        if(t == 1) {
            int l, r; input >> l >> r; ++r;
            print(data(l, r).fold());
        }
        if(t == 2) {
            int p, x; input >> p >> x;
            data[p] = x;
        }
        debug(data.dump_rich());
    }
}
