/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/lcm_convolution"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "numeric/modular/modint.hpp"
#include "convolution/lcm.hpp"

using mint = uni::modint998244353;

signed main() {
    uni::i32 n; input >> n;
    uni::vector<mint> a(n), b(n); input >> a >> b;
    print(uni::lcm_convolution<uni::vector<mint>, 1>(a, b));
}
