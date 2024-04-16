/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/range_reverse_range_sum"


#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adaptor/io.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "action/range_sum.hpp"
#include "action/helpers.hpp"

int main() {
    int n, q; std::cin >> n >> q;
    uni::valarray<uni::i64> a(n); input >> a;
    uni::dynamic_sequence<uni::actions::make_full_t<uni::actions::range_sum<uni::i64>>> data(a);

    REP(q) {
        int t, l, r; std::cin >> t >> l >> r;
        if(t == 0) {
            data.reverse(l, r);
        }
        if(t == 1) {
            print(data(l, r).fold());
        }
    }
}
