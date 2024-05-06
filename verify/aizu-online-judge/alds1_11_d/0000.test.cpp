/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_11_D"

#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/disjoint_set.hpp"

signed main() {
    uni::i32 n, m; input >> n >> m;

    uni::disjoint_set ds(n);

    REP(m) {
        uni::i32 s, t; input >> s >> t;
        ds.merge(s, t);
    }

    uni::i32 q; input >> q;

    REP(q) {
      uni::i32 s, t; input >> s >> t;
      print(ds.same(s, t) ? "yes" : "no");
    }
}
