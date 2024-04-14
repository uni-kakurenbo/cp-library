/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/gcd_convolution"

#include "sneaky/enforce_int128_enable.hpp"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adaptor/io.hpp"
#include "adaptor/vector.hpp"
#include "numeric/modular/modint.hpp"
#include "convolution/gcd.hpp"

using mint = lib::modint998244353;

signed main() {
    int n; std::cin >> n;
    lib::vector<mint> a(n), b(n); input >> a >> b;
    print(lib::gcd_convolution<lib::vector<mint>, 1>(a, b));
}
