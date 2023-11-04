/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <iostream>
#include <utility>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/vector.hpp"
#include "numeric/modint.hpp"
#include "algebraic/affine.hpp"
#include "data_structure/segment_tree.hpp"

using lib::algebraic::affine;
using mint = atcoder::modint998244353;

signed main() {
    int n, q; std::cin >> n >> q;
    lib::vector<lib::spair<int>> f(n); input >> f;

    lib::segment_tree<affine<mint,true>> data(ALL(f));

    LOOP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, a, b; std::cin >> p >> a >> b;
            data.set(p, std::pair<mint,mint>{ a, b });
        }
        if(t == 1) {
            int l, r, x; std::cin >> l >> r >> x;
            auto [a, b] = data.fold(l, r).val();
            print(a * x + b);
        }
    }
}
