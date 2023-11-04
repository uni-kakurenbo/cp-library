/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/point_set_range_composite"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "data_structure/lazy_segment_tree.hpp"
#include "algebraic/affine.hpp"
#include "algebraic/assignment.hpp"


using lib::algebraic::affine;
using mint = atcoder::modint998244353;

struct action {
    using operand = affine<mint,true>;
    using operation = lib::algebraic::assignment<std::pair<mint,mint>>;

    static operand map(const operand& x, const operation& f) { return f->value_or(std::pair<mint,mint>{x->first, x->second}); }
    static operation fold(const operation& x, const lib::internal::size_t) { return x; }
};


signed main() {
    int n, q; std::cin >> n >> q;
    lib::vector<lib::spair<int>> f(n); input >> f;

    lib::lazy_segment_tree<action> data(ALL(f));

    LOOP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, a, b; std::cin >> p >> a >> b;
            data.apply(p, lib::spair<int>{ a, b });
        }
        if(t == 1) {
            int l, r, x; std::cin >> l >> r >> x;
            auto [a, b] = data.fold(l, r).val();
            print(a * x + b);
        }
    }
}
