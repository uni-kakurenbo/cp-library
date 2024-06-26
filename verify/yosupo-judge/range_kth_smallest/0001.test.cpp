/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/range_kth_smallest"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/valarray.hpp"
#include "adaptor/io.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::valarray<uni::u32> a(n); input >> a;

    uni::wavelet_matrix data(a);

    REP(q) {
        uni::i32 l, r, k; input >> l >> r >> k;
        print(*data.range(l, r).kth_smallest_element(k));
    }
}
