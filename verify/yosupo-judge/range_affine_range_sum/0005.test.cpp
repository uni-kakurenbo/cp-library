/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/range_affine_range_sum"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "numeric/modular/modint.hpp"
#include "adaptor/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "action/range_affine_range_sum.hpp"


signed main() {
    using mint = uni::dynamic_montgomery_modint_64bit<-1>;
    mint::set_mod(998244353);

    uni::i32 n, q; input >> n >> q;
    std::vector<mint> a(n); input >> a;

    uni::lazy_segment_tree<uni::actions::range_affine_range_sum<mint>> data(ALL(a));

    REP(q) {
        uni::i32 t; input >> t;
        if(t == 0) {
            uni::i32 l, r, b, c; input >> l >> r >> b >> c;
            data(l, r) *= { b, c };
        }
        if(t == 1) {
            uni::i32 l, r; input >> l >> r;
            print(data(l, r).fold());
        }
    }
}
