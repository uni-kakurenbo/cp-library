/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ITP1_1_A"

#include <cassert>
#include <ranges>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "utility/timer.hpp"
#include "random/engine.hpp"
#include "random/adaptor.hpp"
#include "numeric/prime_sieve.hpp"
#include "numeric/prime_enumerator.hpp"

signed main() {
    print("Hello World");

    lib::random_adaptor<lib::random_engine_32bit> rng;
    lib::timer timer(1000);

    // int i = 0;
    while(not timer.expired()) {
        const int n = rng(1'000'000) + 1;
        // debug(i, n);

        auto p = lib::prime_sieve(n);
        auto q = lib::prime_enumerator(n);

        // debug(p, q);

        assert(std::ranges::equal(p, q));
        assert(std::ranges::equal(p | std::views::reverse, q | std::views::reverse));
    }
}
