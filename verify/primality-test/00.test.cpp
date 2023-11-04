/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/primality_test"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "numeric/fast_prime.hpp"

int main() {
    int q; std::cin >> q;
    REP(q) {
        lib::i64 n; std::cin >> n;
        print.yesno(lib::is_prime(n));
    }
}
