/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_primes"

#include <cassert>
#include <iostream>
#include <ranges>
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "view/stride.hpp"
#include "view/zip.hpp"
#include "numeric/prime_enumerator.hpp"

signed main() {
    int n, a, b; std::cin >> n >> a >> b;
    assert(a > 0);
    auto sieve = lib::prime_enumerator(n);
    print(sieve.size(), (sieve.size() + a - b - 1) / a);
    print(sieve | std::views::drop(b) | lib::views::stride(a));
}
