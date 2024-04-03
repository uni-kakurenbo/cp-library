/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_2_E"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "action/range_add.hpp"
#include "action/helpers.hpp"
#include "algebraic/addition.hpp"
#include "template/debug.hpp"

signed main() {
    int n, q; std::cin >> n >> q;

    lib::dynamic_sequence<
        lib::actions::make_full_t<
            lib::actions::range_add<lib::ll>
        >
    > data(n);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int l, r, x; std::cin >> l >> r >> x; --l;
            data(l, r) += x;
        }
        if(t == 1) {
            int p; std::cin >> p; --p;
            print(data[p]);
        }
        debug(data);
    }

    return 0;
}
