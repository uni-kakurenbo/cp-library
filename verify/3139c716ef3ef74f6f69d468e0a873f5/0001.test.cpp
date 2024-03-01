/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.yosupo.jp/problem/enumerate_quotients"

#include <iostream>
#include <tuple>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "adapter/io.hpp"
#include "numeric/quotient_enumerator.hpp"

signed main() {
    lib::i64 n; std::cin >> n;

    lib::quotient_enumerator qe(n);

    print(qe.size());
    for(auto itr = qe.rbegin(); itr != qe.rend(); ++itr) {
        print << std::get<0>(*itr) << " ";
    }

    print();
}
