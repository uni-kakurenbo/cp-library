/*
 * @uni_kakurenbo
 * https://github.com/uni-kakurenbo/competitive-programming-workspace
 *
 * CC0 1.0  http://creativecommons.org/publicdomain/zero/1.0/deed.ja
 */
/* #language C++ 20 GCC */

#define PROBLEM "https://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_10_C"

#include <iostream>
#include "snippet/aliases.hpp"
#include "snippet/fast_io.hpp"
#include "snippet/iterations.hpp"
#include "adapter/io.hpp"
#include "adapter/string.hpp"
#include "iterable/longest_common_subsequence.hpp"

signed main() {
    int q; std::cin >> q;
    REP(q) {
        lib::string x, y; std::cin >> x >> y;
        print(lib::lcs_sizes(ALL(x), ALL(y)).back().back());
    }
}
