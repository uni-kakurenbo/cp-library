/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/static_range_frequency"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "adaptor/valarray.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::valarray<unsigned> a(n); input >> a;

    uni::wavelet_matrix data(a);

    REP(q) {
        uni::i32 l, r, x; input >> l >> r >> x;
        print(data(l, r).count(x));
    }
}
