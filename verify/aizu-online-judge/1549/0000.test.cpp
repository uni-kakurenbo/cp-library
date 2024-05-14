/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=1549"

#include <bits/stdc++.h>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/valarray.hpp"
#include "adaptor/io.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    uni::i32 n; input >> n;
    uni::valarray<uni::i32> a(n); ITRR(v, a) input >> v;

    uni::wavelet_matrix<uni::ll> data(a);
    // debug(data);

    uni::i32 q; input >> q;
    REP(q) {
        uni::i32 l, r, x; input >> l >> r >> x; ++r;
        print(
            std::min(
                std::abs(data(l, r).prev(x).value_or(INT64_MAX) - x),
                std::abs(data(l, r).next(x).value_or(INT64_MAX) - x)
            )
        );
    }
}
