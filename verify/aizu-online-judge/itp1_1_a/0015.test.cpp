/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "adaptor/io.hpp"
#include "numeric/modular/modint.hpp"
#include "data_structure/dynamic_sequence.hpp"
#include "data_structure/red_black_tree.hpp"
#include "action/range_affine_range_sum.hpp"

#include "verify/aizu-online-judge/itp1_1_a/internal/dynamic_sequence.0001.hpp"

using mint = uni::modint998244353;

signed main() {
    print("Hello World");

    test<
        mint,
        uni::dynamic_sequence<
            uni::actions::range_affine_range_sum<mint>,
            uni::treap_context<uni::i128>
        >,
        uni::dynamic_sequence<
            uni::actions::range_affine_range_sum<mint>,
            uni::persistent_red_black_tree_context<uni::i128>
        >
    >(30'000, 10'000);
}
