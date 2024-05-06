/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=DSL_1_A"

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/disjoint_set.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;

    uni::disjoint_set ds(n);

    REP(q) {
        uni::i32 t, x, y; input >> t >> x >> y;
        if(t == 0) {
            ds.merge(x, y);
        }
        if(t == 1) {
            print(ds.same(x, y));
        }
    }
}
