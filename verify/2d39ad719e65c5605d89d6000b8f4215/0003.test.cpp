/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/dynamic_sequence_range_affine_range_sum"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "numeric/modular/modint.hpp"
#include "adapter/vector.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "action/range_affine_range_sum.hpp"

using mint = lib::static_arbitrary_montgomery_modint_32bit<998244353>;

signed main() {
    int n, q; std::cin >> n >> q;
    lib::vector<mint> a(n); input >> a;

    lib::dynamic_sequence<lib::actions::range_affine_range_sum<mint>> data(a);

    REP(q) {
        int t; std::cin >> t;
        if(t == 0) {
            int p, x; std::cin >> p >> x;
            data.insert(p, mint{ x });
        }
        if(t == 1) {
            int p; std::cin >> p;
            data.erase(p);
        }
        if(t == 2) {
            int l, r; std::cin >> l >> r;
            data.reverse(l, r);
        }
        if(t == 3) {
            int l, r, b, c; std::cin >> l >> r >> b >> c;
            data(l, r) += { b, c };
        }
        if(t == 4) {
            int l, r; std::cin >> l >> r;
            print(data(l, r).fold());
        }
    }
}
