/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/convolution_mod_1000000007"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "numeric/modular/modint.hpp"
#include "convolution/sum.hpp"

using mint = lib::modint1000000007;

signed main() {
    int n, m; std::cin >> n >> m;
    lib::vector<lib::u64> a(n), b(m); input >> a >> b;
    print(lib::convolution<lib::vector<mint>>(a, b));
}
