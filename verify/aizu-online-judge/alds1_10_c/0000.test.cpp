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
#include "adaptor/io.hpp"
#include "adaptor/string.hpp"
#include "iterable/longest_common_subsequence.hpp"

signed main() {
    uni::i32 q; input >> q;
    REP(q) {
        uni::string x, y; input >> x >> y;
        print(uni::lcs_sizes(ALL(x), ALL(y)).back().back());
    }
}
