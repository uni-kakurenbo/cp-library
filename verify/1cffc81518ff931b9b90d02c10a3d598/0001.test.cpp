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
#include "adapter/io.hpp"
#include "adapter/valarray.hpp"
#include "data_structure/wavelet_matrix.hpp"

signed main() {
    int n, q; std::cin >> n >> q;
    lib::valarray<unsigned> a(n); input >> a;

    lib::wavelet_matrix data(a);

    REP(q) {
        int l, r, x; std::cin >> l >> r >> x;
        print(data(l, r).count(x));
    }
}
