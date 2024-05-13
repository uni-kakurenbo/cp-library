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
#include "adaptor/io.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "numeric/modular/modint.hpp"
#include "action/range_set_range_composition.hpp"


using mint = uni::modint998244353;

signed main() {
    int n, q; std::cin >> n >> q;
    std::vector<std::pair<mint,mint>> f(n); input >> f;

    uni::dynamic_sequence<uni::actions::range_set_range_composition<mint>> data(f);

    REP(i, q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, a, b; std::cin >> p >> a >> b;
            if(i % 2) data[p] = { a, b };
            else data[p] *= std::make_pair(a, b);
        }
        if(t == 1) {
            int l, r, x; std::cin >> l >> r >> x;
            auto [a, b] = data.fold(l, r).val();
            print(a * x + b);
        }
    }
}
