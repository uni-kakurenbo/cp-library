/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */


#define PROBLEM "https://judge.yosupo.jp/problem/factorize"

#include "sneaky/enforce_int128_enable.hpp"

#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "numeric/fast_prime.hpp"

signed main() {
    int q; std::cin >> q;
    REP(q) {
        lib::ll a; std::cin >> a;
        auto facts = lib::factorize(a);
        print(facts.size(), facts);
    }
}
