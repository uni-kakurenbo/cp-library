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
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "numeric/modular/modint.hpp"
#include "algebraic/affine.hpp"
#include "data_structure/segment_tree.hpp"

using uni::algebraic::affine;
using mint = uni::modint998244353;

signed main() {
    uni::i32 n, q; input >> n >> q;
    uni::vector<uni::spair<uni::i32>> f(n); input >> f;

    uni::segment_tree<affine<mint>> data(ALL(f));

    LOOP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 p, a, b; input >> p >> a >> b;
            data.set(p, std::pair<mint,mint>{ a, b });
        }
        if(t == 1) {
            uni::i32 l, r, x; input >> l >> r >> x;
            auto [a, b] = data.fold(l, r).val();
            print(a * x + b);
        }
    }
}
